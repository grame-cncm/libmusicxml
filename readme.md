# LibMusicXML Antescofo Project
This is a private fork of the original [Grame LibMusicXML Library](https://github.com/grame-cncm/libmusicxml) from GitHub, for Antescofo's internal use.

Before any use, make note of the following facts:
- The main branch of this repository is `antescofo-master`. This branch is used to deploy to the _GuidoKit_ private Pod for iOS Projects.
- Other branches such as `master` and `dev` are used to track the original Grame project and update `antescofo-master` whenever appropriate.
- We use *Pull Requests* on `antescofo-master` branch

## Installation

- After cloning the project, open terminal and `cd` to the `build` folder. 
- Run the `BuildXCodeProject.sh` in terminal. This will create appropriate `iOS` and `MacOS` XCode projects and folders.

## Deploy to Pod

Once the XCode Projects are ready, just run the `deploy2pod.sh` from the `build` folder. This will compile necessary objects, and copy appropriate libraries and sources to the _Pod Project Path_ given as argument.

## Contribution

Always branch out from `antescofo-master` and create pull-requests to that branch for Antescofo internal use.

## Upgrading the Library from Grame

Ideally you should have a secondary remote called `grame` pointing to `https://github.com/grame-cncm/libmusicxml.git`. The `dev` branch on that repository contains latest additions and upgrades.
To upgrade latest contributions from Grame, create a new Branch and merge those contributions with that of `antescofo-master` and contribute as usual by create a Pull Request.