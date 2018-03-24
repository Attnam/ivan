Iter Vehemens ad Necem
==========================

[![Build Status](https://travis-ci.org/Attnam/ivan.svg?branch=master)](https://travis-ci.org/Attnam/ivan)
[![Build status](https://ci.appveyor.com/api/projects/status/lsx9yspma48ag0fr?svg=true)](https://ci.appveyor.com/project/Attnam/ivan)

[![Community](https://img.shields.io/badge/forum-cathedral%20of%20attnam-yellow.svg)](https://attnam.com/)
[![Latest Release](https://img.shields.io/github/release/attnam/ivan/all.svg)](https://github.com/Attnam/ivan/releases/tag/v052)

[![Latest Release Downloads](https://img.shields.io/github/downloads/attnam/ivan/v052/total.svg)](https://attnam.com/projects/official/IVAN-Continuation)
[![Commits Since Latest Release](https://img.shields.io/github/commits-since/attnam/ivan/v052.svg)](https://github.com/Attnam/ivan/compare/v052...master)
[![Total Downloads](https://img.shields.io/github/downloads/attnam/ivan/total.svg)](https://attnam.com/projects/official/IVAN-Continuation)

Iter Vehemens ad Necem (IVAN) is a graphical roguelike game, which currently
runs in Windows, DOS, Linux, and OS X. It features advanced bodypart and
material handling, multi-colored lighting and, above all, deep gameplay.

This is a fan continuation of IVAN by members of Attnam.com

Contributing
--------------------------

This project adheres to the Contributor Covenant [code of conduct](CODE_OF_CONDUCT.md).
By participating, you are expected to uphold this code. Please report any unacceptable behavior.

Contributions are welcome! Please fork this repository and then once you have 
made and tested your changes, submit a pull request. It will be reviewed by 
one of the admins as soon as possible.

Forum
--------------------------

Join us on Attnam.com for discussion of planned features, etc. (or to yell at 
us if we neglect your pull request for too long).

--------------------------

*From the original README...*

General gameplay
-----------------------------

IVAN works pretty much in the same way as other roguelikes. The player
controls a character, which moves and attacks from the direction keys.
All other commands can be found by pressing ?-key in the game.

Additional help can be obtained from the unofficial site at
http://attnam.com

FAQ
-----------------------------

**Q:** What does "Iter Vehemens ad Necem" mean?  
**A:** It's Latin and means a "Violent Road to Death". For most players, that's
   a perfect description of the typical game.

**Q:** Why can't I make multiple saves and why is my save deleted when I die?  
**A:** Like the creators of other roguelikes, we think this makes gaming much
   more exciting, since you must take full responsibility of all your
   actions. You have only one chance to live or die. Also, we agree that
   "normal" saveloading is OK for games which remain the same in all gaming
   sessions, since in these you are not meant to really die as replaying
   everything in exactly the same way would be annoying. But the same does
   not apply to games with a multitude of random areas and events like IVAN;
   the whole fun is trying again and again in everchanging environment,
   encountering stranger and more complex situations and becoming better 
   and better in tackling them.

**Q:** Can't you reconsider your opinion about saveloading?  
**A:** No. There are the two things we swore never to do when starting the
   project: discarding permadeath and making IVAN a 3D action game. Don't
   bother us about this question anymore.

**Q:** Not even as an "easy" game option? Pretty please?  
**A:** Shut up.

**Q:** What do these strange markings like Dex, Agi mean in the right sidebar?  
**A:**  
   AStr = Arm Strength - Increases damage inflicted on enemies  
   LStr = Leg Strength - Increases carrying capacity and kicking damage  
   Dex = Dexterity - Increases accuracy and hit speed  
   Agi = Agility - Increase movement speed and ability to dodge attacks  
   End = Endurance - Increases maximum health points and healing rate  
   Per = Perception - Increases sight range and accuracy  
   Int = Intelligence - Increases your ability to read and use magic  
   Wis = Wisdom - Increases your ability to deal with gods  
   Cha = Charisma - Improves your ability to negotiate and make deals  

   If there are two numbers visible after these the first is the attribute
   after bonuses and penalties obtained from your equipment or some other
   temporary reason. The second number shows the base attribute without the
   said modifications. If there is no net-bonus or net-minus, only the base
   attribute is shown.

   If the first number is green, then it has increased a short while ago and 
   if it is red it has decreased.

   Siz = Size or height, in cm - Increases your enemies' chance to hit you  
   HP = Health Points - The sum of your bodyparts' HPs  
   Gold = The amount of money you have  
   Day & Time = The amount of absolute game time you have spent in this game  
   Turn = The turns (commands which take time) you have used in this game  

**Q:** I had 20 HPs, but I still died. Is this a bug?  
**A:** No, this is not a bug. You will die (at least in human-form) if the HPs
   of your groin, torso or head reach 0.

**Q:** Why do I always miss spiders with my trusty iron mace?  
**A:** Mace is far too big for hitting small creatures. Do you really kill
   spiders with such enormous objects in real life?

**Q:** What do marks like L+, C-- mean in the pray menu?  
**A:** All gods have a property that tells whether they are supporters of Law
   or Chaos or if they are Neutral. This is however too general description
   so the plusses and minuses tell of slight differences between the gods.
   Eg Valpurus has the letters L++ this means that Valpurus is extremely
   lawful, when Sophos has the letters L-, which in turn means that Sophos
   is lawful, but still leaning a bit towards neutrality and even chaos.

**Q:** I just lost a leg. How do I get it back?  
**A:** Certain gods, potions and special characters may help you.

**Q:** How is score calculated?  
**A:** First the kills of the player and his pets are merged to a single
   list. Then for every monster type, the score is calculated with the
   following formula

```
Score = sqrt(a) * b * b
```

   sqrt(a) = Square root of the number of killed monsters of this type  
   b = Sum of the attributes of a typical monster of this type

   The individual scores of the monster types are then added together.
   You also get a high bonus for winning, depending on your victory type
   (the score is multiplied by 2, 3, 4 or 5).

**Q:** Why does time pass so fast in the game?  
**A:** Who has said the a day is as long in IVAN's world as on earth?

**Q:** How can I compile IVAN source code?  
**A:** See INSTALL.

**Q:** I am a Linux / OS X user. Why can't I access the wizard (cheat) mode?  
**A:** The wizard mode functions aren't compiled by default. Change
   the environment variable CXXFLAGS to -DWIZARD and recompile.

**Q:** I am a DOS user. When I try to run IVAN, I get the message "Load error:
   no DPMI - Get csdpmi\*b.zip".  
**A:** The DOS binary is compiled using DJGPP, so you need a DPMI server to run
   it. One should have been provided along with IVAN, but if that's not the
   case, download it from www.delorie.com.

**Q:** I've found a bug. What should I do?  
**A:** Write a small description on how the bug occurred and if possible even
   how we could replicate it, and send this information by email to
   ivan-support@googlegroups.com. You can also report this bug on our
   forum:

   http://attnam.com

   You should mention your full name so we can credit you at the end of the
   AUTHORS file, if you are the first to discover this bug.

**Q:** I've got a great idea to make IVAN better! What should I do?  
**A:** Describe the idea to us by sending it to ivan-support@googlegroups.com
   or report it on our forum (see above). You will be credited if we
   implement the feature, it's non-trivial, and you're first to suggest it.

**Q:** I'm a programmer willing to help you. What should I do?  
**A:** Join us on Github! https://github.com/Attnam/ivan
   Contributions are welcome. Fork the repository and then once you have
   made and tested your changes, submit a pull request. It will be reviewed
   by one of the admins as soon as possible.

   See `.customs.emacs` file for the official Ivan C++ Programming
   Style. 

-----------------------------

FREE SOFTWARE FOREVER!
