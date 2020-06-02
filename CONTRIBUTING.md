# Contribution guidelines

__Things to do__

0) Adhere to the [Code of Conduct](https://github.com/Attnam/ivan/blob/master/CODE_OF_CONDUCT.md).
1) Join the [Attnam.com](https://attnam.com) forums, say hello. Figure out the changes the community wants to see, [check here as a starting point](https://attnam.com/wiki/Development_plans).
2) Fix bugs before coding more bugs.
3) Code new stuff while avoiding to code bugs.
4) Release early and release often.
5) Make experimental contributions while revising or deleting stuff that isn't working.

__Awesome jobs__

 - Fixing bugs and improving stability.
 - Coding new subsystems, for example bodypart mutation subsystem, new dangers for player characters.
 - Improving continuous integration and both cloud- and ground-based compiling systems for all supported platforms.
 - Scripting new dungeons and new places for players to go.
 - Commenting the code, document the danger system, weapon balance and combat system.

__Forbidden jobs__

 - Intentionally creating bugs.
 - Upsetting game balance.
 - Adding/modding weapons without knowing how the weapon system is balanced.
 - Adding content that breaches the [Code of Conduct](https://github.com/Attnam/ivan/blob/master/CODE_OF_CONDUCT.md).
 - Telepathically communicating with Elpuri.


# Commits, pull requests (PRs) and merging

__Commit messages__

[Follow the guideline found here and you will be sweet.](https://chris.beams.io/posts/git-commit/)

__Pull requests__

There are two ways to do a pull request

1) One way: _Draft_ a pull request. Once it is ready to be reviewed/merged, change status from draft to actual PR.

2) Another way: Make the pull request and prefix the title with [WIP] (work in progress) denoting that it is not ready to be merged. Once it is ready to be reviewed/merged, change prefix to [Ready].

__Merging__

There are also two ways to do this. Both ways are equally polite.

1) Review the PR first. Someone other than the author should do this. If the PR is approved by another dev, then anyone else including the PR author can merge.

2) Just merge the PR. The review step and merge step gets rolled into one. Only someone other than the author should do this. 

__Merging your own work__

Sometimes it is necessary to merge your own work. Times when it is fine to merge your own work include:
 - Another dev approved your PR.
 - You are fixing any bug or fatal error, particularly one you authored.
 - You are trying to push a tag, i.e. trigger a release. Usually done by the release person, but could conceivably be done by anyone.
 - You are the only dev left on the project.

