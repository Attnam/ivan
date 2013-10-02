                   Iter Vehemens ad Necem v. 0.231 alpha

                     (eli Väkivaltainen Tie Kuolemaan)

            Copyright (C) Fatal Error Productions (FEP) 1999-2001

----------------------------------------------------------------------------

                     Dokumentaatio betatestaajia varten

----------------------------------------------------------------------------

Aluksi
------

Kiitos, että vaivauduit imuroimaan IVAN:n. Jos et vielä tiedä, millainen
peli sinua odottaa, paljastamme kyseessä olevan uuden Rogue-tyylisen pelin,
eli keinotekoisen maailman, jossa pelaaja liikkuu pitkin joka kerta
uudelleen luotavaa syvää luolastoa piesten hirviöarmaadoja, joita
tulee matkalla vastaan. Jos olet pelannut Nethackia tai ADOM:ia aiemmin,
tiedät varmasti mistä on kyse.

Olemme FEP:ssä käyttäneet paljon aikaa tämän teoksen tuottamiseen, ja
luulemme version olevan jo kohtuullisen nautittavaa pelattavaa. Siksi
toivomme, että testaisit peliä kohtuullisen tuokion ja kertoisit meille
mielipiteesi. Tarvitsemme palautetta siitä, mikä on jo hyvää ja toisaalta
siitä mitä pitäisi parantaa, ideoita uusista ominaisuuksista, joita peliin
olisi syytä lisätä, ja tietoa mahdollisista virheistä pelissä.
Jälkimmäisistä kaikkein pahimpia ovat sellaiset, jotka kaatavat pelin,
tekevät siitä läpipeluukelvottoman tai päinvastoin antavat mahdollisuuden
tarkoituksettoman helppoon pelaajan ominaisuuksien boostaamiseen. Ilmoita
HETI jos sellaisia löytyy!

TÄRKEINTÄ KAIKESSA!
-------------------

Kun olet pelannut tarpeeksi, lähetä palautteesi ja löytämiesi bugien lista
jompaankumpaan seuraavista osoitteista:

sonor@phpoint.fi
Timo Kiviluoto, IVAN-ohjelmoijatiimin pääorganisaattori

hejosa@mantta.fi
Heikki Sairanen, varaorganisaattori ja markkinointijohtaja

----------------------------------------------------------------------------

Laitteistovaatimukset
---------------------

Teoreettinen vähimmäisvaatimus on PC ja sen sisällä i386-prosessori tai
uudempi. Todellisuudessa täysin jökkimättömään kokemukseen tarvitset ainakin
300 MHz:n härvelin ja jonkun hitusen järkevämmän tyypinkin sille. Tämä on
kenties liikaa Rogue likelle, mutta tässä vaiheessa emme ole katsoneet
optimointia pääprioriteetiksi.

Koska pääohjelmoijallamme meni ammoin guruus päähän ja hän vaati saada
tehdä itse kaikki projektin grafiikkafunktiot konekielellä,
näytönohjaintuessa voi ilmetä ongelmia korteissa, jotka eivät ole
yhteensopivia Vesa 2.0:n kanssa tai tue resoluution 800x600 16-bittistä
tilaa. Yhdestäkään sellaisesta näytönohjaimesta emme tosin ole vielä
kuulleet.

Käyttöjärjestelmistä Win95 ja uudemmat sekä DOS 7.0 on testattu toimiviksi.
Tulevissa versioissa myös Linuxia tullaan tukemaan.

----------------------------------------------------------------------------

Pelaamisesta
------------

Liikkuminen
-----------

IVAN toimii niin kuin suurin osa Rogue likeista, eli pelaajan hahmoa
liikutellaan pitkin luolastoa nuolinäppäimillä (numpad on paras vaihtoehto).
Jos vastaan tulee hirviö, sitä voi lyödä yksinkertaisesti kävelemällä
(jälleen nuolinäppäimistä) hirviötä kohti.

Oven tullessa vastaan sitä päin voi kävellä ja ohjelma kysyy avatataanko se.

Lista erikoiskomennoista
------------------------

Pelissä on muita enemmän tai vähemmän hyödyllisiä funktioita, jotka on
listattu tähän. Pelissä listan saa myös esille, painamalla ?-näppäintä.

Huomaa, että esim. näppäimet 'd' ja 'D' tekevät täysin eri asioita, eli
shiftin tai caps lockin tila on huomioitava!

Komennon nimi (näppäin) = Selitys.

"consume" (e) = Syö/juo itemeitä. Ohjelma kysyy, minkä itemin haluat
käyttää tähän tarkoitukseen. Myös suoraan maasta syöminen onnistuu.

"show inventory" (i) = Näytä hahmon kantamat itemit.

"pick up" (,) = Nosta tavara maasta. Jos tavaroita on useampia, näytä
valikko, josta käyttäjä voi valita nostettavan.

"quit" (q) = Lopeta peli. (Tätähän et tarvitse eikö niin?)

"drop" (d) = Pudota itemi.

"wield" (w) = Ota ase käteen.

"go up" (<) = Mene ylös portaita.

"go down" (>) = Mene alas portaita.

"open" (o) = Avaa ovi tai tavara (esim. can).

"close" (c) = Sulje ovi.

"wear armor" (W) = Pue jonkin vaate tai armori päälle.

"talk" (t) = Puhu.

"wait" (.) = Odota vuoro, lepää.

"save" (s) = Tallenna peli ja lopeta.

"read" (r) = Lue pergamentti tai joku muu tavara.

"dip" (D) = Dippaa, kasta.

"increase gamma" (g) = Lisää gammaa. (gamma = melkein valoisuus... ei
ihan mutta melkein). Huom. tämä toimii ehkä noin joka kolmannella koneella.

"decrease gamma" (G) = Äskeisen vastakohta.

"show key layout" (?) = Näytä näppäimet.

"look" (l) = Tarkastele maastoa, monstereita, itemeitä.

"engrave" (E) = Kaiverra maahan. (Täysin turhaa, mutta hauskaa :)

"pray" (y) = Rukoile jumalilta apua.

"kick" (k) = Potkaise jotakin.

"take screenshot" (S) = Kuvankaappaus, jos haluat jostain syystä laittaa
kuvan parhaasta pelistäsi kotisivullesi tai jotain. 

"offer" (O) = Uhraa. Toimii ainoastaan alttarilla.

"increase software gamma" (f) = Vähennä gammaa softapohjaisesti. Toimii
kaikilla koneilla.

"decrease software gamma" (F) = Äskeisen vastakohta.

Huijaajille
-----------

Seuraavat näppäimet on tarkoitettu lähinnä tiettyjen ominaisuuksien
nopeaan testaamiseen tuotannon aikana, ja ne ovat täysin turhia normaalille
pelaajalle. Listaamme ne kuitenkin varmuuden vuoksi.

"wizard mode" (X) = Aktivoi huijaus-moodi. Huom! Tämä on peruuttamaton
toiminto, joka samalla evää highscore-listalle pääsyn.

"raise stats" (R) = Nosta pelaajahahmon atribuutteja.

"lower stats" (T) = Laske pelaajahahmon atribuutteja.

"see whole map" (Y) = Näytä koko kartta. Tämä on varsin hidasta.

"toggle walk through walls cheat" (U) = Toglaa päälle seinienläpikävely-
huijauksen.

----------------------------------------------------------------------------

Taustaa
-------

IVAN sijoittuu 'Terra Valpuria'-universumiin, uskonnolliseen maailmaan,
jossa jumaluudet ovat erittäin merkittävässä asemassa ja papisto mahtava.
Pelaaja on nuori valpuristinoviisi, jonka Suuri Valpurin Ylipappi Perttu
lähettää vaaralliselle tehtävälle tappamaan kauhean Elpurin, Pimeän
Sammakon, ja sen lukuisat pahuudenkyllästämät palvelijat. Tosin pelissä
on myös vaihtoehtoisia, vaikeampia ja nautinnollisempia voittamistapoja
tarpeeksi mahtaville ja Valpurin arvossa pitämille... Yritäpä löytää jokin
niistä; jos siihen pystyt, olet aika kova ivannaaja!

OK, tiedetään, juoni on pelkkä sisäpiirivitsi tällä hetkellä, mutta se
tullaan uudistamaan. Keskitty pelin engineen äläkä välittä niin paljoa
oudosta huumoristamme.

----------------------------------------------------------------------------

Selviytymisguide
----------------

Ensi kertaa pelaavalle IVAN saattaa tuntua vaikeahkolta, sillä taktiikat
eroavat monin tavoin muista genren peleistä. Olemme tähän koonneet
tärkeimmät saannöt alkuun pääsemiseksi.

I
Pysy alussa Pertun lähellä, sillä vanhurskaana valpuristina hän auttaa
nuorempaansa monin tavoin. Ensimmäinen kenttä on muutenkin paljon helpompi
kuin seuraavat; jos sinulla on ongelmia siellä niin älä mene alaspäin!

II
Syö vatsasi täyteen, mutta älä enempää. Mutta muista ruokaillessasi:
Olet valpuristi, etkä saa navastaa.

III
Rukoile kun on tarvis. Hyvät jumalat ovat alussa hyödyllisempiä (Atavusta
ja Valpuria lukuunottamatta, he vaativat erittäin hyviä suhteita ennen kuin
antavat mitään). Veniuksen pyhän tulen, Dulciksen mesmerisoivan laulun ja
Consummon avaruudenmuuntelujen avulla selviät helposti piiritystilanteista
ja Seges auttaa jos ruokaa pääsee loppumaan. Kaaottisia jumaliakin voit
kokeilla, he antavat joskus suuriakin lahjoja, mutta suuttuessaan ovat
erittäin vaarallisia.

IV
Käytä jokaista hirviötä vastaan sitä asetta, jota oikeassa elämässä
käyttäisit. Et varmasti mene pieksemään pientä pimeää sammakkoa kahdenkäden
miekalla, ethän osu mihinkään. Sen sijaan yrität rutistaa sen käsin.
Tämä on erittäin tärkeää huomata, sillä joihinkin monsuihin ei alussa kerta
kaikkiaan voi osua isoilla ja painavilla aseilla.

V
Jos olet haavoittunut, älä lähde heti berserker-hyökkäykseen, vaan odota,
kunnes HP on melko lähellä maksimia. Muuten saat itsesi varmasti hengiltä.

VI
Jos hirviö on liian vaikea pala sinulle, juokse! Olet usein nopeampi kuin
vastustajasi. Jos et, toisaalta, ainoa vaihtoehto on eksytys (monsut on
tyhmiä kuin vihannekset) tai teleportaatio taian tai Consummolle rukoilun
avulla.

VI
Jos löydät hyvän haarniskan, pistä se päälle heti kun jaksat kantaa sen.
Pudota muita tavaroita, jos se on välttämätöntä.

VII
Älä ikinä ja missään tapauksessa taistele burdened-tilassa tai huonommassa.
IVAN:ssa tästä koituva rangaistus on aivan liian suuri.

VIII
Muista aina expottaa hillittömästi.

----------------------------------------------------------------------------

Hyvää ja voittoisaa pelaamista, toivottaa FEP!