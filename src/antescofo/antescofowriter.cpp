/*
  Copyright (C) 2013 Thomas Coffy

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  thomas.coffy@gmail.com
  http://repmus.ircam.fr/antescofo
*/

#include "antescofowriter.h"


namespace MusicXML2
{

map<rational, measure_elt>::iterator antescofowriter::findNoteInVector(rational atbeat, rational dur) 
{
	map<rational, measure_elt>::iterator i;
	if (((i = v_Notes.find(atbeat))) != v_Notes.end())
		return i;
	else { // if exact note doesn't exist, maybe a note, began before
		for (i = v_Notes.begin(); i != v_Notes.end(); i++) {
			if (i->second.m_pos <= atbeat // note could be before
					&& i->second.m_pos + i->second.duration > atbeat)
				return i;
		}
	}
	return v_Notes.end();
}

// search for beat&measure in measure2beat map, 
// if does not exist
//   //if prevmeasure has beat
//   add
// else return real beat.
rational antescofowriter::AddBeat(rational beat, int nmeasure) {
	map<int, rational>::iterator i;
	if ((i = measure2beat.find(nmeasure)) == measure2beat.end()) { // not found
		/*
		//|| measure2beat[nmeasure + 1] > measure2beat[]) 
		map<int, float>::iterator p;
		if ((i = measure2beat.find(nmeasure-1)) != measure2beat.end()) { // not found
		*/
		measure2beat[nmeasure] = beat;
		return beat;

	} else { // found
		//cout << "AddBeat: got beat associated to measure: beat " << i->second.toFloat() << " and measure " << i->first << endl;
		map<int, rational>::iterator n, p;
		// if beatprev < beat < beatnext return beat
		if ((n = measure2beat.find(nmeasure+1)) != measure2beat.end()) {
			if (n->second > beat) {// && beat >= i->second) // if between current measure and next measure's beat

				if ((p = measure2beat.find(nmeasure-1)) != measure2beat.end()) {
					if (p->second < beat)//|| beat >= i->second)
						return beat;
				} 
				// return beat;
			}
		}
		if (beat >= i->second)
			return beat;
		return i->second;
	}
}

// curBeat is relative to the measure
// so if nmeasure>1 we suppose notes were added before,
// and the curBeat in absolute beats can be found.
void antescofowriter::AddNote(int type, float pitch, rational dur, float nmeasure, rational &curBeat, int flag_, string rehearsal) {
	map<rational, measure_elt>::iterator i;
	cout << "; Addnote(beat:"<<curBeat.getNumerator() << "/" << curBeat.getDenominator() << ", meas:" << nmeasure <<" pitch:"<<pitch << " dur:"<< dur.getNumerator()<<"/"<<dur.getDenominator() << " type:"<<  type << " bpm:"<<fBPM<<") ";
	//cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl; print(false);

	cout << "AddBeat: beat:" << curBeat.toFloat() << " measure " << nmeasure << endl;
	curBeat = AddBeat(curBeat, nmeasure);
	cout << "AddBeat: ==> curBeat:" << curBeat.toFloat() << " measure " << nmeasure << endl;
#if 1
	rational abs_curBeat(0);
	if (nmeasure > 1 && curBeat.toFloat() == 1.) {
		if (pitch == 0.)
			return;
		cerr << "AntescofoWriter: something went wrong with note beats, trying to figure out current beat from measure number." << endl;
		for (map<rational, measure_elt>::const_iterator i = v_Notes.begin(); i != v_Notes.end(); i++) {
			if (i->second.nMeasure == nmeasure) {
				abs_curBeat = i->second.m_pos + curBeat - rational(1);
				break;
			}
		}
		if (abs_curBeat.getNumerator()) {
			curBeat = abs_curBeat;
			cout << "; Addnote(beat changed to :"<<curBeat.toFloat()<<endl;
		} else {
			// try to find nearest measure, and add beats...
			rational diffb = 0;
			for (map<rational, measure_elt>::const_iterator i = v_Notes.begin(); !abs_curBeat.toFloat() && i != v_Notes.end(); i++) {
				for (int diffm = 1; diffm != 4; diffm++) {
					if (i->second.nMeasure - diffm == nmeasure) {
						diffb += i->second.duration;
						for (int d = 0; d != diffm && i != v_Notes.end(); d++, i--) {
							diffb += i->second.duration;
						}
						abs_curBeat = i->second.m_pos + diffb;
						break;
					}
				}
			}
			if (abs_curBeat.getNumerator()) {
				curBeat = abs_curBeat;
				cout << "; Addnote(beat interpolated changed to :"<<curBeat.toFloat()<<endl;
			}
			else {
				cerr << "AntescofoWriter: something went wrong with note beats, check your MusicXML file, sorry." << endl;
				print(false);
				antescofo_abort();
			}
		}
	}
#endif
	if (curBeat.getNumerator() == 1. && nmeasure > 1)  { antescofo_abort(); }
	if (flag_ == ANTESCOFO_FLAG_TIED_START) cout << "tie=Start";
	if (flag_ == ANTESCOFO_FLAG_TIED_END) cout << "tie=End";
	// find note in vector
	if (( i = findNoteInVector(curBeat, dur)) == v_Notes.end()) { // new note
		measure_elt *e = new measure_elt();
		e->nMeasure = nmeasure;
		e->type = type;
		dur.rationalise();
		e->duration = dur;
		e->m_pos = curBeat;
		e->flags = flag_;
		if (flag_ == ANTESCOFO_FLAG_FERMATA) e->bFermata = true;
		if (fLastBPM != fBPM) { fLastBPM = fBPM; e->bpm = fBPM; }
		if (type == ANTESCOFO_NOTE || type == ANTESCOFO_CHORD || type == ANTESCOFO_REST) {
			if (type != ANTESCOFO_REST) {
				if (dur.toFloat() == .0) // grace
					e->grace_pitches.push_back(pitch);
				else e->pitches.push_back(pitch);
			}   
			cout << "; AddNote: adding single note: " << curBeat.toFloat() <<endl;
		} else if (type == ANTESCOFO_TRILL) {
			e->pitches.push_back(pitch);
			cout << "; AddNote: TRILL note: " << curBeat.toFloat() <<endl;
		} else if (type == ANTESCOFO_MULTI) {
			e->pitches.push_back(pitch);
			cout << "; AddNote: MULTI note: " << curBeat.toFloat() <<endl;
		}
		v_Notes[curBeat] = *e;
		cout << endl;
	} else { // note already exists at this position
		if (type == ANTESCOFO_REST) return;

		if (i->second.m_pos != curBeat) { // if note was there, but not on the exact same beat
			cout << "!!!!!!!!!!!! Inserting note in the middle of a previous one. !!!!!!!!!!!!!!" << endl;
			cout << "<<<<<<<< WTF: " << i->second.m_pos.getNumerator() << "/" << i->second.m_pos.getDenominator() << " curBeat:" << curBeat.getNumerator() << "/" << curBeat.getDenominator() << " prevdur:"<< i->second.duration.toFloat() << " dur:" << dur.toFloat() << endl;
			if (i->second.type == ANTESCOFO_REST) { // XXX only handle REST for now
				rational d = i->second.duration;
				cout << "!!!!!!!!!!!! Inserting note: previous note: beat:"<< i->second.m_pos.toFloat() << " dur:"<< i->second.duration.toFloat() << endl;
				if (i->second.m_pos.getNumerator() == 0 && i->second.duration.getNumerator() == 0) return;
				i->second.duration = curBeat - i->second.m_pos;//rational(curBeat*1000, 1000) - rational(i->second.m_pos * 1000, 1000);// shorten previous note duration
				cout << "!!!!!!!!!!!! Inserting note: reducing first one dur:"<< i->second.duration.toFloat() << endl;
				if (i->second.duration + dur < d) { // if prev dur < dur add note
					rational tmpcurbeat = curBeat + dur;//.toFloat();
					rational bkpbeat = curBeat;
					cout << "!!!!!!!!!!!!  Inserting note: dur:"<< (d - i->second.duration - dur).toFloat() << " atbeat:"<< tmpcurbeat.toFloat() << " !!!!!!!!!!!!!!" << endl;
					if (((d - i->second.duration - dur).toFloat()) <= 0 ) return; 
					AddNote(ANTESCOFO_REST, 0, d - i->second.duration - dur, nmeasure, tmpcurbeat);
					curBeat = bkpbeat;
				} // XXX > ?
				dur.rationalise();
				AddNote(type, pitch, dur, nmeasure, curBeat);
				return;
			}

		}

		if (i->second.duration == dur || dur.toFloat() == .0) { // if already present note's duration is same than new one, merge it
			if (i->second.type == ANTESCOFO_REST) { // replace REST by NOTE
				if (dur.toFloat() == .0) { // grace
					cout << "Replace rest by grace note" << endl;
					i->second.grace_pitches.push_back(pitch);
					i->second.type = type;
				} else {
					cout << "Replace rest by note" << endl;
					i->second.pitches.push_back(pitch);
					i->second.type = type;
					dur.rationalise();
					i->second.duration = dur;
				}
				i->second.nMeasure = nmeasure;
				i->second.m_pos = curBeat;
				i->second.flags = flag_;
				if (flag_ == ANTESCOFO_FLAG_FERMATA) i->second.bFermata = true;
				return;
			} else if (type != ANTESCOFO_REST && (i->second.type == ANTESCOFO_NOTE || i->second.type == ANTESCOFO_CHORD)) { // change NOTE to CHORD
				if (i->second.duration.toFloat() == .0) // previous note was a grace note, so just add note.
				{
					if (dur.toFloat() == .0) {
						i->second.grace_pitches.push_back(pitch);
						if (i->second.grace_pitches.size() > 1)
							i->second.type = ANTESCOFO_CHORD;
					} else 
						i->second.pitches.push_back(pitch);
					dur.rationalise();
					i->second.duration = dur;
					i->second.nMeasure = nmeasure;
					i->second.m_pos = curBeat;
					i->second.flags = flag_;
					if (flag_ == ANTESCOFO_FLAG_FERMATA) i->second.bFermata = true;
					cout << "; Addnote : handling grace note." << endl;
					return;
				}
			}
			// else :
			i->second.type = ANTESCOFO_CHORD;
			if (dur.toFloat() == .0) // handle grace notes
				i->second.grace_pitches.push_back(pitch);
			else i->second.pitches.push_back(pitch);
			i->second.nMeasure = nmeasure;
			i->second.m_pos = curBeat;
		} else { // if already present note's duration is different of new one, create it.
			/*
				 i->second.type = ANTESCOFO_CHORD;
				 i->second.pitches.push_back(pitch);
				 i->second.nMeasure = nmeasure;
				 i->second.m_pos = curBeat;
				 */

			rational new_beat = curBeat + i->second.duration;
			int new_measure = nmeasure;
			rational new_dur = i->second.duration - dur;
			if (new_dur > rational(0)) { // note (or chord) already in the list is longer than new note: new note is shorter
				cout << "; Addnote : expanding note: new_dur:" << new_dur.toFloat();
				i->second.type = ANTESCOFO_CHORD;
				i->second.pitches.push_back(pitch);
				i->second.nMeasure = nmeasure;
				i->second.m_pos = curBeat;

				// on the beat: old + new
				i->second.duration = dur;
				cout << " shorten note: beat "<< i->second.m_pos.toFloat() << ", dur: "<< i->second.duration.toFloat() << " and new beat:"<< (curBeat+i->second.duration).toFloat()<<  ", dur:"<<new_dur.toFloat()<< endl;

				for (vector<int>::iterator c = i->second.pitches.begin(); c != i->second.pitches.end() && *c != pitch; c++) {
					cout << "=============== ADDING NOTE: "<< *c << " b:"<< (new_beat - new_dur).toFloat()<< endl;
					rational tmpcurbeat = new_beat - new_dur;
					new_dur.rationalise();
					AddNote(ANTESCOFO_CHORD, -(*c), new_dur, new_measure, tmpcurbeat);
				}
			} else { // new note is longer than already present one, so copy present pitches into new note with new_dur duration
				new_dur = rational(0) - new_dur;
				cout << "; Addnote : expanding new longer note: new_dur:" << new_dur.toFloat() << endl;

				if (i->second.type == ANTESCOFO_REST) { // replace REST by NOTE, then add REST with diff duration
					cout << "replace REST by NOTE, then add REST with diff duration"<<endl;
					i->second.pitches.push_back(pitch);
					i->second.type = type;
					i->second.duration = dur;
					rational newbeat = curBeat + dur;
					// check if next note(?s) needs to be shortened
					map<rational, measure_elt>::iterator n = i;
					n++;
					if (n != v_Notes.end() && n->second.type == ANTESCOFO_REST) {
						cout << "erasing " << newbeat.toFloat() << " SILENCE " << endl;
						v_Notes.erase(n->first);
						new_dur.rationalise();
						AddNote(ANTESCOFO_REST, 0, new_dur, nmeasure, newbeat);
					}
				} else if (i->second.duration.toFloat() == 0) { // handle grace notes
					if (i->second.pitches.size())
						i->second.grace_pitches.push_back(i->second.pitches[0]);
					i->second.type = type;
					i->second.pitches.push_back(pitch);
					i->second.duration = dur;

				} else {
					// on the beat: old + new
					//if (i->second.duration = ANTESCOFO_REST) i->second.duration = dur; else
					i->second.duration = MIN(i->second.duration, dur);
					i->second.type = ANTESCOFO_CHORD;
					i->second.pitches.push_back(pitch);
					i->second.nMeasure = nmeasure;
					i->second.m_pos = curBeat;

					// on the beat+prevdur: only new
					measure_elt *e = new measure_elt();
					e->type = (v_Notes[curBeat].pitches.size() + 1 > 1 ? ANTESCOFO_CHORD : ANTESCOFO_NOTE);
					new_dur.rationalise();
					e->duration = new_dur;
					rational tmpbeat = curBeat + i->second.duration; tmpbeat.rationalise();
					for (vector<int>::iterator c = v_Notes[tmpbeat].pitches.begin(); c != v_Notes[tmpbeat].pitches.end(); c++)
						e->pitches.push_back(-(*c));
					e->pitches.push_back(-pitch);
					e->nMeasure = nmeasure;
					e->m_pos = tmpbeat;
					e->flags = flag_;
					if (flag_ == ANTESCOFO_FLAG_FERMATA) e->bFermata = true;
					if (fLastBPM != fBPM) { fLastBPM = fBPM; e->bpm = fBPM; }
					v_Notes[tmpbeat] = *e;// XXX que nenni si la note existe deja a cette position
				}
			}
		}

	}
	v_Notes[curBeat].rehearsal = rehearsal;
}

void antescofowriter::print(bool with_header) {
	writestream(cout, with_header);
}


void antescofowriter::write(const char *outfilename) {
	ofstream outfile;

	outfile.open(outfilename);
	writestream(outfile, true);
	outfile.close();
}



void antescofowriter::setSelectedParts(vector<string> parts)
{
	write_parts = parts;
}

void antescofowriter::setSelectedStaves(vector<string> staves)
{
	for (vector<string>::iterator v = staves.begin(); v != staves.end(); v++) cout << "Not converting staff : "<<*v<< endl;
	write_staves = staves;
}

void antescofowriter::setSelectedVoices(vector<string> voices)
{
	write_voices = voices;
	//for (vector<string>::iterator v = voices.begin(); v != voices.end(); v++) cout << "$$$$$$$$$$$$$ voices: "<<*v<< endl; antescofo_abort();
}

void antescofowriter::setSelectedMeasures(vector<int> measures)
{
	write_measures = measures;
}



// final processing :
// - compress for each note, if the following note is the same pitch, and if they belong to the same measure
//   add durations and suppress next
// - compress tied notes
void antescofowriter::final_compress()
{
	// - compress
	if (v_Notes.size() < 2) return;
	map<rational, measure_elt>::iterator next = v_Notes.begin();
	map<rational, measure_elt>::iterator i = next;
	next++;
	for (; next != v_Notes.end(); next++) {
		if (next->second.nMeasure == i->second.nMeasure && next->second.type == i->second.type) {
			if ((/*next->second.type == ANTESCOFO_NOTE ||*/ next->second.type == ANTESCOFO_REST)
					&& next->second.pitches.size() && i->second.pitches.size() && next->second.pitches[0] == i->second.pitches[0]) {
				merge_notes(i, next);
			} else if ((next->second.type == ANTESCOFO_TRILL || next->second.type == ANTESCOFO_MULTI || next->second.type == ANTESCOFO_CHORD)
					&& i->second.pitches == next->second.pitches) {
				merge_notes(i, next);
			}
		}
		i = next;
	}

	// - compress tied notes
	if (v_Notes.size() < 2) return;
	i = next = v_Notes.begin();
	next++;
	for (; next != v_Notes.end(); next++) {
		if (next->second.type == i->second.type && next->second.pitches == i->second.pitches
				&& i->second.flags == ANTESCOFO_FLAG_TIED_START && next->second.flags == ANTESCOFO_FLAG_TIED_END) {
			cout << "Got tied notes (pos:"<<i->first.toFloat()<<")... merging note." <<endl;
			merge_notes(i, next);
		}
		i = next;
	}

}


// merge 2 notes a and b (add durations) in a, and delete b
void antescofowriter::merge_notes(map<rational, measure_elt>::iterator a, map<rational, measure_elt>::iterator b) {
	cout << "merge_notes " << a->first.toFloat() << " and " << b->first.toFloat() << endl;
	if (a->second.bpm.size()) {
		if (b->second.bpm.size())
			return; // do nothing if two note are separated by different tempos
	} else if (b->second.bpm.size())
		a->second.bpm = b->second.bpm;
	if (a->second.nMeasure != b->second.nMeasure) { //tied note between different measures
		//cout << "-------- jumping next measure: "<< a->second.nMeasure << "  ----------" << endl; 
		//a->second.nMeasure++;
	}
	a->second.duration += b->second.duration;
	v_Notes.erase(b);
}



void antescofowriter::antescofo_abort() {
	cout << "Antescofo abort: -------------- An error occured, leaving. ----------------" << endl;
	print(false);
	abort();
}

void antescofowriter::print_duration(ostream &out, rational &du) {
	if (du.getNumerator() == 0) {
		out << "0";
	} else {
		out << du.getNumerator();
		if (du.getDenominator() != 1) {
			out << "/";
			out << du.getDenominator();
		}
	}
}

void antescofowriter::writenote(ostream &out, int pitch) {
	if (!print_notes_names) {// || pitch % 100 != 0) {
		out << pitch << "00";
	} else {
		string prefix = "";
		if (pitch < 0) {
			prefix = "-";
			pitch = -pitch;
		}
		if (pitch > 1000) pitch /= 100;
		int p = pitch % 12;
		int o = 0;
		if (pitch >= 12 && pitch <= 23) o = 0;
		if (pitch >= 24 && pitch <= 35) o = 1;
		if (pitch >= 36 && pitch <= 47) o = 2;
		if (pitch >= 48 && pitch <= 59) o = 3;
		if (pitch >= 60 && pitch <= 71) o = 4;
		if (pitch >= 72 && pitch <= 83) o = 5;
		if (pitch >= 84 && pitch <= 95) o = 6;
		if (pitch >= 96 && pitch <= 107) o = 7;
		if (pitch >= 108 && pitch <= 120) o = 8;
		string c = "";
		if (p == 0) c = "C";
		if (p == 1) c = "C#";
		if (p == 2) c = "D";
		if (p == 3) c = "D#";
		if (p == 4) c = "E";
		if (p == 5) c = "F";
		if (p == 6) c = "F#";
		if (p == 7) c = "G";
		if (p == 8) c = "G#";
		if (p == 9) c = "A";
		if (p == 10) c = "A#";
		if (p == 11) c = "B";
		out << prefix << c << o;
	}
}


			
void antescofowriter::writestream(ostream &out, bool with_header) {
	//expand();
	//std::sort(v_Notes.begin(), v_Notes.end());
	//cout << "notes sz:" << v_Notes.size() << endl;
	bool bNewMeasure = false;
	bool bPendingFermata = false;
	if (with_header) {
		final_compress();
		out << "; Antescofo score generated using libmusicxml and embedded xml2antescofo converter." <<std::endl;
		out << "; Copyright (c) Thomas Coffy - IRCAM 2013" <<std::endl;
	}
	if (v_Notes.size()) fBPM = v_Notes.begin()->second.bpm;
	if (!fBPM.size()) fBPM = string("120");
	fLastBPM = fBPM;
	out << "BPM " << fBPM << endl;
	int cur_meas = 0;
//	rational fCurrBeat = rational(1);

	int last_glissando_pitch = 0;
	for (map<rational, measure_elt>::iterator i = v_Notes.begin(); i != v_Notes.end(); i++) { 
		if (i->second.nMeasure == 0 && i->second.m_pos.getNumerator() == 0 && i->second.duration.getNumerator() == 0)
			continue;

		// check we want to write this measure
		bool bad = write_measures.empty() ? false : true;
		for (vector<int>::const_iterator v = write_measures.begin(); v != write_measures.end(); v++) {
			if (*v == i->second.nMeasure)
				bad = false;
		}
		if (bad) continue;

		// display measure comment
		if (cur_meas != i->second.nMeasure) {
			bNewMeasure = true;
			cur_meas = i->second.nMeasure;
			out << endl << "; ----------- measure " << cur_meas << " --- beat " << i->first.getNumerator();
			if (i->first.getDenominator() != 1)
				out << "/" << i->first.getDenominator()<< " ------";
			out << endl;
		}
		if (i->second.bFermata) {
			out << "TEMPO OFF" << endl;
			bPendingFermata = true;
		} else if (bPendingFermata) {
			out << "TEMPO ON" << endl;
			bPendingFermata = false;
		}
		if (i->second.bpm.size() && i->second.bpm != fBPM) {
			fBPM = i->second.bpm ;
			out << "BPM " << fBPM << " @modulate" << endl; 
		}
		if (i->second.duration.getNumerator()) i->second.duration.rationalise();

		// grace notes
		int grace_notes = i->second.grace_pitches.size();
		if (grace_notes) {
			//if (grace_notes == 1) { out << "NOTE "; writenote(out, i->second.grace_pitches[0]); out << " 0"; }
			//else if (grace_notes > 1) {
			//	out << "CHORD ( ";
			for (vector<int>::const_iterator j = i->second.grace_pitches.begin(); j != i->second.grace_pitches.end(); j++) {
				out << "NOTE "; writenote(out, *j); out << " 0" << endl;
				//writenote(out, *j);//out << *j << "00 "; out << ") 0";// << endl;
			}
			//}
			//if (i->second.pitches.size()) out << endl; // don't print new measure label on grace notes
		} 
		if (i->second.type == ANTESCOFO_NOTE && i->second.pitches.size() && i->second.pitches[0]) {
			out << "NOTE "; writenote(out, i->second.pitches[0]); out << " ";
			print_duration(out, i->second.duration);
		} else if (i->second.type == ANTESCOFO_REST) {
			out << "NOTE 0 ";
			print_duration(out, i->second.duration);
		}
		else if (i->second.pitches.size() && i->second.pitches[0]) {
			// sort
			std::sort(i->second.pitches.begin(), i->second.pitches.end());
			// remove duplicates pitches
			vector<int>::iterator u = std::unique(i->second.pitches.begin(), i->second.pitches.end());
			i->second.pitches.resize(u - i->second.pitches.begin());

			if (i->second.type != ANTESCOFO_MULTI) {
				if (i->second.type == ANTESCOFO_CHORD) {
					if (i->second.pitches.size() == 1) 
						out << "NOTE";
					else 
						out << "CHORD (";
				} else if (i->second.type == ANTESCOFO_TRILL)
					out << "TRILL (";
				last_glissando_pitch = 0;
			} else { // glissando flags may be: start, stop, stop
				if (last_glissando_pitch && i->second.flags == ANTESCOFO_FLAG_GLISSANDO_STOP) {
					out << "MULTI ( "; writenote(out, last_glissando_pitch);
					out << " ->";
				}
				if (i->second.flags == ANTESCOFO_FLAG_GLISSANDO_START || i->second.flags == ANTESCOFO_FLAG_GLISSANDO_STOP)
					last_glissando_pitch = i->second.pitches[0];
			}
			for (vector<int>::const_iterator j = i->second.pitches.begin(); j != i->second.pitches.end(); j++) {
				if (i->second.flags != ANTESCOFO_FLAG_GLISSANDO_START) {
					out << " "; writenote(out, *j);// << "00";
				}
				//if (i->second.type == ANTESCOFO_MULTI && i->second.flags == ANTESCOFO_FLAG_GLISSANDO_START) out << " ->";
			}

			if (i->second.flags != ANTESCOFO_FLAG_GLISSANDO_START) {
				if (i->second.pitches.size() > 1 || i->second.type == ANTESCOFO_TRILL || i->second.flags == ANTESCOFO_FLAG_GLISSANDO_STOP) 
					out << " )";
				out << " ";
				print_duration(out, i->second.duration);// << i->second.duration.getNumerator() << "/" << i->second.duration.getDenominator();
			}
		}
		if (bNewMeasure && i->second.flags != ANTESCOFO_FLAG_GLISSANDO_START) {
			out << " measure"<< cur_meas;;
			bNewMeasure = false;
		}
		if (i->second.rehearsal.size()) 
			out << " ; marker: " << i->second.rehearsal;

		if (i->second.flags != ANTESCOFO_FLAG_GLISSANDO_START) {
			out << endl;
		}
	}
}

} // namespace MusicXML2 

