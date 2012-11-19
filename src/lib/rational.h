/*
  Copyright © Grame 2003

  This library is free software; you can redistribute it and modify it under
  the terms of the GNU Library General Public License as published by the
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  research@grame.fr
  
*/

#ifndef __rational__
#define __rational__

#include <string>
#include "exports.h"

using namespace std;

/*!
\brief	Rational number representation.
*/

class EXP rational {

   private:    
   
        long int fNumerator;
        long int fDenominator;        
        
        // Used by rationalise()
        long int gcd(long int a, long int b); 
 
    public:    
	
        rational(long int num = 0, long int denom = 1);
        rational(const rational& d);
        rational(const string &str);
    
        long int getNumerator()	const		{ return fNumerator; }
        long int getDenominator() const		{ return fDenominator; }
        void setNumerator(long int d)		{ fNumerator = d; }
        void setDenominator(long int d) 	{ fDenominator = d; }
		void set(long int n, long int d)  { fNumerator = n; fDenominator = d; }

        rational operator +(const rational &dur) const;
        rational operator -(const rational &dur) const;
        //! Useful for notes with dots.
        rational operator *(const rational &dur) const; 
        rational operator /(const rational &dur) const;
        // (i.e. dur * 3/2 or dur * 7/4)
  
        rational operator *(int num) const; 
        rational operator /(int num) const;
  
        rational& operator +=(const rational &dur);
        rational& operator -=(const rational &dur);
        //! Useful for notes with dots.
        rational& operator *=(const rational &dur); 
        rational& operator /=(const rational &dur);
        // (i.e. dur * 3/2 or dur * 7/4)

        rational& operator *=(long int num) { fNumerator *= num; return *this; }
        rational& operator /=(long int num) { fDenominator *= num; return *this; }
 
        rational& operator =(const rational& dur);
    
        bool operator >(const rational &dur) const;
        bool operator >=(const rational &dur) const 	{return !(*this < dur);}
        bool operator <(const rational &dur) const;
        bool operator <=(const rational &dur) const 	{return !(*this > dur);}
            
        bool operator ==(const rational &dur) const;
        bool operator !=(const rational &dur) const	{return !(*this == dur);}
      
        bool operator >	(double num) const;
        bool operator >=(double num) const;
        bool operator <	(double num) const;
        bool operator <=(double num) const;    
        bool operator ==(double) const;

        // Used to "rationalise" rational.
        void rationalise();
       
        operator string () const;
		operator double () const;
		operator float () const;
		operator int () const; 

        //virtual string toString() const;  Why ?? SL
		string toString() const;
        double	toDouble() const;
        float	toFloat() const;
        int		toInt() const;
};

#endif
