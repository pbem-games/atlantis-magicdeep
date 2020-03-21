//---------------------------------------------------------------------------


#pragma hdrstop

#include "game.h"
#include "gamedata.h"
#include "NameGen.h"
//#include <system.hpp>
//#include <Classes.hpp>

#pragma package(smart_init)

AString 	aPrefAbstract[] = {
"A",
"Ab",
"Ach",
"Ad",
"Ae",
"Ag",
"Ai",
"Ak",
"Al",
"Am",
"An",
"Ap",
"Ar",
"As",
"Ash",
"At",
"Ath",
"Au",
"Ay",
"Ban",
"Bar",
"Brel",
"Bren",
"Cam",
"Cla",
"Cler",
"Col",
"Con",
"Cor",
"Cul",
"Cuth",
"Cy",
"Chal",
"Chan",
"Chi",
"Chon",
"Chul",
"Chur",
"Del",
"Dur",
"Dwar",
"Dwur",
"Ek",
"El",
"En",
"Eth",
"Fal",
"Far",
"Fel",
"Fell",
"Fen",
"Flan",
"Flar",
"Fly",
"Fur",
"Fy",
"Gal",
"Gan",
"Gar",
"Gel",
"Glan",
"Glar",
"Glen",
"Glir",
"Glyn",
"Glyr",
"Glyth",
"Gogh",
"Gor",
"Goth",
"Gwal",
"Gwen",
"Gwur",
"Gy",
"Gyl",
"Gyn",
"Ghal",
"Ghash",
"Ghor",
"Ghoz",
"Ghul",
"Hach",
"Haj",
"Hal",
"Ham",
"Hel",
"Hen",
"Hil",
"Ho",
"Hol",
"Hul",
"Ice",
"Id",
"Ie",
"Il",
"Im",
"In",
"Ir",
"Is",
"Iz",
"Ja",
"Jak",
"Jar",
"Jaz",
"Jeth",
"Jez",
"Ji",
"Jul",
"Jur",
"Juz",
"Kag",
"Kai",
"Kaj",
"Kal",
"Kam",
"Ken",
"Kor",
"Kul",
"Kwal",
"Kwar",
"Kwel",
"Kwen",
"Kha",
"Khel",
"Khor",
"Khul",
"Khuz",
"Lagh",
"Lar",
"Lin",
"Lir",
"Loch",
"Lor",
"Lyn",
"Lyth",
"Mal",
"Man",
"Mar",
"Me",
"Mer",
"Meth",
"Mil",
"Min",
"Mir",
"Nam",
"Nar",
"Nel",
"Nem",
"Nen",
"Nor",
"Noth",
"Nyr",
"Ob",
"Oe",
"Ok",
"Ol",
"On",
"Or",
"Ow",
"Par",
"Pel",
"Por",
"Py",
"Pyr",
"Pyl",
"Ral",
"Ra",
"Ram",
"Rath",
"Re",
"Rel",
"Ren",
"Ri",
"Ril",
"Ro",
"Ror",
"Ruk",
"Ry",
"Sen",
"Seth",
"Sul",
"Shae",
"Shal",
"Shar",
"Shen",
"Shir",
"Tal",
"Tam",
"Tar",
"Tel",
"Ten",
"Tir",
"Tol",
"Tul",
"Tur",
"Thor",
"Thul",
"U",
"Uk",
"Un",
"Ul",
"Va",
"Val",
"Van",
"Vel",
"Ven",
"Veren",
"Vul",
"Wal",
"War",
"We",
"Wel",
"Wil",
"Win",
"Y",
"Ya",
"Ych",
"Ye",
"Yg",
"Yi",
"Yl",
"Yn",
"Yo",
"Yp",
"Yr",
"Yth",
"Yu",
"Yul",
"Za",
"Zar",
"Zel",
"Zi",
"Zim",
"Zir",
"Zol",
"Zor",
"Zhok",
"Zhu",
"Zhuk",
"Zhul"
};
AString 	aSufAbstract[] = {
"a",
"ach",
"aech",
"ael",
"aem",
"aen",
"aer",
"aeth",
"ail",
"ain",
"air",
"aith",
"all",
"an",
"and",
"ar",
"ash",
"auch",
"aul",
"aun",
"aur",
"baen",
"bain",
"bar",
"bath",
"ben",
"byr",
"cael",
"caer",
"can",
"cen",
"cor",
"cynd",
"dach",
"dail",
"dain",
"dan",
"dar",
"dik",
"dir",
"dy",
"e",
"eal",
"el",
"eld",
"eth",
"gar",
"gath",
"grim",
"i",
"ian",
"ield",
"ien",
"ieth",
"il",
"ior",
"ioth",
"ish",
"maer",
"mail",
"main",
"mar",
"maren",
"miel",
"mieth",
"nain",
"nair",
"naith",
"nal",
"nar",
"nath",
"nen",
"ner",
"niel",
"nien",
"nieth",
"nor",
"noth",
"nul",
"nur",
"nyr",
"o",
"och",
"or",
"oth",
"oum",
"owen",
"rach",
"raid",
"rail",
"rain",
"raish",
"raith",
"ran",
"rar",
"ras",
"raven",
"ren",
"riel",
"rien",
"rier",
"rik",
"ril",
"rish",
"ron",
"ror",
"ros",
"roth",
"rych",
"ryl",
"ryr",
"rych",
"thach",
"thain",
"thak",
"thal",
"than",
"thar",
"thiel",
"thien",
"thor",
"thul",
"thur",
"ug",
"uild",
"uin",
"uith",
"uk",
"ul",
"un",
"ur",
"uth",
"wain",
"waith",
"wald",
"war",
"ward",
"well",
"wen",
"win",
"y",
"yll",
"ynd",
"yr",
"yth",
"zak",
"zel",
"zen",
"zokh",
"zor",
"zul",
"zuth"
};

AString 	aPrefEscimo[] = {
"ach",
"achana",
"achka",
"achuk",
"ak",
"akip",
"akun",
"ani",
"aninnik",
"ap",
"apat",
"api",
"apucha",
"chat",
"chit",
"chua",
"chut",
"ichis",
"ichitt",
"ikissik",
"ikiutta",
"iniki",
"iniss",
"inissik",
"ippik",
"issi",
"it",
"ita",
"itani",
"kass",
"kikk",
"kiun",
"kyap",
"kyaun",
"kyu",
"kyup",
"kuk",
"kup",
"kupp",
"kut",
"pi",
"pyaun",
"pyi",
"pyu",
"pyuin",
"pyup",
"siss",
"syaun",
"syiin",
"syu",
"syuss",
"syuun",
"saun",
"suss",
"suun",
"tap",
"tikk",
"tya",
"tyaan",
"tyiun",
"tuan",
"uch",
"uchach",
"uchk",
"ucht",
"upani",
"upik",
"utin",
"unuchut"};
AString 	aSufEscimo[] = {
"ach",
"akan",
"akya",
"ani",
"anu",
"chach",
"chat",
"chin",
"chit",
"chun",
"iakap",
"ichak",
"ikin",
"ikip",
"ikta",
"in",
"innik",
"ipa",
"ippik",
"ipuch",
"issik",
"it",
"itut",
"kan",
"kip",
"kuk",
"kukik",
"kya",
"pich",
"pin",
"pip",
"pput",
"sup",
"tait",
"tin",
"tip",
"tit",
"titut",
"uchi",
"uk",
"ukta",
"utta"};

AString 	aPrefGreek[] = {
"aer",
"agamen",
"agor",
"aion",
"air",
"aker",
"akrogon",
"akro",
"amen",
"ametan",
"amiant",
"ampel",
"anan",
"anankamom",
"andro",
"aner",
"antano",
"aorat",
"apolytro",
"athem",
"autark",
"biast",
"byblo",
"chrono",
"dogmo",
"dokim",
"ekbalo",
"ekpipto",
"ektrom",
"entell",
"epikatar",
"ereun",
"exang",
"exod",
"gorgo",
"hekono",
"heter",
"hikan",
"hilar",
"hymno",
"hypno",
"ianno",
"ierem",
"kalym",
"katar",
"klepto",
"kreman",
"makar",
"malak",
"maran",
"metan",
"nest",
"oikonom",
"optan",
"orgil",
"otar",
"ouran",
"papyr",
"parait",
"paramen",
"parthen",
"perik",
"peril",
"perim",
"philag",
"polyl",
"poro",
"prax",
"sin",
"sken",
"smyrno",
"strato",
"thron",
"trit",
"troglo",
"zelot"};
AString 	aSufGreek[] = {
"akos",
"alizo",
"alotos",
"arenos",
"aros",
"arotes",
"arx",
"askalos",
"atos",
"atres",
"einos",
"elos",
"eros",
"eryx",
"etes",
"imos",
"irmos",
"itos",
"okles",
"opos",
"otus"
"polis"
"us"};

AString 	aPrefAztec[] = {
"Acayu",
"Alar",
"Apatzin",
"Ayoquez",
"Ayu",
"Cham",
"Chetu",
"Chi",
"Cho",
"Chun",
"Colo",
"Comalcal",
"Comi",
"Cuet",
"Hala",
"Huicha",
"Huimax",
"Hunuc",
"Ix",
"Ixmiquil",
"Iza",
"Jal",
"Jamil",
"Juchi",
"Kaminal",
"Kantunil",
"Maya",
"Mapas",
"Maxcan",
"Maz",
"Miahu",
"Minatit",
"Mul",
"Noch",
"Oax",
"Oco",
"Ome",
"Ozibilchal",
"Panab",
"Pet",
"Pochu",
"Popoca",
"Say",
"Sayax",
"Tehuan",
"Tenoxtit",
"Tep",
"Tik",
"Tiz",
"Tizi",
"Tlaco",
"Tom",
"Ton",
"Tul",
"Tun",
"Tux",
"Uaxac",
"Urua",
"Yaxchi",
"Zacat",
"Zana",
"Zima"};
AString 	aSufAztec[] = {
"atlan",
"ixtlan",
"huas",
"juyu",
"poton",
"talpan",
"tepec",
"tepetl",
"titlan",
"zalan"};

AString	aPrefDrow[] = {
"Alean",
"Ale",
"Arab",
"Arken",
"Auvry",
"Baen",
"Barri",
"Cladd",
"Desp",
"De",
"Do’",
"Eils",
"Everh",
"Fre",
"Gode",
"Helvi",
"Hla",
"Hun",
"Ken",
"Kil",
"Mae",
"Mel",
"My",
"Noqu",
"Orly",
"Ouss",
"Rilyn",
"Teken'",
"Tor",
"Zau"};
AString aSufDrow[] = {
"afin",
"ana",
"ani",
"ar",
"arn",
"ate",
"ath",
"duis",
"ervs",
"ep",
"ett",
"ghym",
"iryn",
"lyl",
"mtor",
"ndar",
"neld",
"rae",
"rahel",
"rret",
"sek",
"th",
"tlar",
"t’tar",
"tyl",
"und",
"urden",
"val",
"virr",
"zynge"};

AString	aPrefScotish[] = {
"Aber",
"Ar",
"As",
"At",
"Avie",
"Bal",
"Ben",
"Bran",
"Brech",
"Bro",
"Cairn",
"Can",
"Carl",
"Colon",
"Clyde",
"Craig",
"Cum",
"Dearg",
"Don",
"Dor",
"Dun",
"Dur",
"El",
"Fal",
"For",
"Fyne",
"Glas",
"Hal",
"Inver",
"Ju",
"Kil",
"Kilbran",
"Kirrie",
"Lairg",
"Lin",
"Lo",
"Loch",
"Lorn",
"Lyb",
"Ma",
"Mal",
"Mel",
"Monadh",
"Nairn",
"Nith",
"Ob",
"Oron",
"Ran",
"Scar",
"Scour",
"Spey",
"Stom",
"Strom",
"Tar",
"Tay",
"Ti",
"Tober",
"Uig",
"Ulla",
"Wick"};
AString	aSufScotish[] = {
"aline",
"an",
"aray",
"avon",
"ba",
"bert",
"bis",
"blane",
"bran",
"da",
"dee",
"deen",
"far",
"feldy",
"gin",
"gorm",
"ie",
"in",
"kaig",
"kirk",
"laig",
"liath",
"maol",
"mond",
"moral",
"more",
"mory",
"muir",
"na",
"nan",
"ner",
"ness",
"nhe",
"nock",
"noth",
"nure",
"ock",
"pool",
"ra",
"ran",
"ree",
"res",
"say",
"ster",
"tow"};

AString	aPrefAfrica[] = {
"Ag",
"Ahr",
"Ba",
"Bor",
"Dar",
"Don",
"Dor",
"Dung",
"Ga",
"Gal",
"Gam",
"Gul",
"Gur",
"Gwa",
"Gwah",
"Gwar",
"Gwul",
"Ig",
"Ja",
"Jih",
"Jug",
"Kas",
"Kesh",
"Kides",
"Kili",
"Kor",
"Kul",
"Kush",
"Lar",
"Lu",
"Ma",
"Mat",
"Mbeg",
"Mbeng",
"Min",
"Ngor",
"Ngul",
"N'Gul",
"Nyag",
"N'Yag",
"N'Zin",
"Ong",
"Rod",
"Sha",
"Sum",
"Swa",
"Ti",
"Tot",
"Ug",
"Ung",
"Wad",
"Waz",
"Wur",
"Ya",
"Za",
"Zang",
"Zar",
"Zem",
"Zik",
"Zim",
"Zu",
"Zul"};
AString	aSufAfrica[] = {
"a",
"ad",
"aga",
"ara",
"ai",
"al",
"alo",
"ang",
"anga",
"ani",
"bab",
"bal",
"balla",
"biba",
"bu",
"buk",
"buru",
"daja",
"dar",
"donga",
"dor",
"du",
"dul",
"duru",
"daza",
"'guba",
"'gung",
"hili",
"i",
"id",
"iji",
"ili",
"jari",
"jaro",
"juri",
"'ka",
"lah",
"lur",
"mala",
"mim",
"mu",
"munga",
"mur",
"nur",
"nuzi",
"o",
"od",
"ofo",
"oja",
"onga",
"ozi",
"ra",
"sala",
"sula",
"sunga",
"tulo",
"u",
"ula",
"ulga",
"unga",
"wa",
"wath",
"we",
"wuzi",
"zaja",
"zaza",
"zin",
"zum",
"zung",
"zur"};

AString	aPrefElven1[] = {
"Ama",
"Ari",
"Aza",
"Cla",
"Cy",
"Dae",
"Dho",
"Dre",
"Fi",
"Ia",
"Ky",
"Lue",
"Ly",
"Mai",
"My",
"Na",
"Nai",
"Nu",
"Ny",
"Py",
"Ry",
"Rua",
"Sae",
"Sha",
"She",
"Si",
"Tia",
"Ty",
"Ya",
"Zy"};
AString	aSufElven1[] = {
"nae",
"lae",
"dar",
"drimme",
"lath",
"lith",
"lyth",
"lan",
"lanna",
"lirr",
"lis",
"lys",
"lyn",
"llinn",
"lihn",
"nal",
"nin",
"nine",
"nyn",
"nis",
"sal",
"sel",
"tas",
"thi",
"thil",
"vain",
"vin",
"wyn",
"zair"};

AString	aPrefElven2[] = {
"Aer",
"Al",
"Am",
"Ang",
"Ansr",
"Ar",
"Arn",
"Bael",
"Cael",
"Cal",
"Cas",
"Cor",
"Eil",
"Eir",
"El",
"Er",
"Ev",
"Fir",
"Fis",
"Gael",
"Gil",
"Il",
"Kan",
"Ker",
"Keth",
"Koeh",
"Kor",
"Laf",
"Lam",
"Mal",
"Nim",
"Rid",
"Rum",
"Seh",
"Sel",
"Sim",
"Syl",
"Tahl",
"Vil"};
AString	aSufElven2[] = {
"ael",
"aer",
"aera",
"aias",
"aia",
"aith",
"aira",
"ala",
"ali",
"ani",
"uanna",
"ari",
"aro",
"ibrar",
"adar",
"odar",
"udrim",
"emar",
"esti",
"evar",
"afel",
"efel",
"ihal",
"ihar",
"ahel",
"ihel",
"ian",
"ianna",
"iat",
"iel",
"ila",
"inar",
"ine",
"ith",
"elis",
"ellon",
"inal",
"anis",
"aruil",
"eruil",
"isal",
"sali",
"sar",
"asar",
"isar",
"asel",
"isel",
"itas",
"ethil",
"avain",
"avin",
"azair"};

AString	aPrefDwarven[] = {
"agar",
"agaz",
"barak",
"baruk",
"baraz",
"bizar",
"bizul",
"bul",
"buzar",
"garak",
"gor",
"gog",
"gorog",
"gothol",
"guzib",
"ibin",
"ibiz",
"izil",
"izuk",
"kelek",
"kezan",
"kibil",
"kinil",
"kun",
"kheled",
"khelek",
"khimil",
"khuz",
"laruk",
"luz",
"moran",
"moril",
"nibin",
"nukul"};
AString	aSufDwarven[] = {
"akar",
"agul",
"amen",
"gib",
"gol",
"gog",
"gul",
"guluth",
"gundil",
"gundag",
"guzun",
"lib",
"lizil",
"loth",
"mab",
"mor",
"mud",
"mur",
"nazar",
"nigin",
"niz",
"nizil",
"nuz",
"nuzum",
"thibil",
"thizar",
"ulin",
"uzar",
"uzun",
"zad",
"zakar",
"zal",
"zalak",
"zam",
"zan",
"zaral",
"zarak",
"zeg",
"zerek",
"zibith",
"zikil",
"zokh",
"zukum"};

AString	aPrefOrchish[] = {
"Arg",
"Az",
"Bad",
"Balkh",
"Bol",
"Dreg",
"Dur",
"Durba",
"Ghash",
"Lurg",
"Luz",
"Mor",
"Nazg",
"Og",
"Tarkh",
"Urg",
"Ug",
"Vol",
"Yazh"};
AString	aSufOrchish[] = {
"agal",
"buz",
"dor",
"dur",
"gar",
"mog",
"narb",
"nazg",
"rod",
"shak",
"waz",
"ubal"};

AString	aPrefArabic[] = {
"Aaza",
"Abha",
"Ad",
"Aga",
"Ah",
"Ain",
"Ait",
"Ajda",
"Ali",
"Al",
"Arrer",
"As",
"Ash",
"Ay",
"Az",
"Bab",
"Bani",
"Bari",
"Bat",
"Birak",
"Bitam",
"Bou",
"Dakh",
"Dha",
"Dham",
"Djaz",
"Djeb",
"Fash",
"Ghad",
"Ghar",
"Ghat",
"Gra",
"Had",
"Ham",
"Har",
"Jawf",
"Jer",
"Jid",
"Jir",
"Kabir",
"Kebir",
"Ket",
"Khat",
"Khem",
"Kher",
"Khum",
"Ksar",
"Mak",
"Mara",
"Men",
"Mu",
"Qat",
"Qay",
"Sa",
"Sab",
"Sah",
"Sal",
"Sidi",
"Sma",
"Sulay",
"Tabel",
"Tar",
"Tay",
"Taza",
"Ubay",
"Wah",
"Yab",
"Yaf",
"Yous",
"Zil",
"Zou"};
AString	aSufArabic[] = {
"ada",
"ah",
"air",
"ama",
"amis",
"aq",
"ar",
"ash",
"at",
"bala",
"biya",
"dah",
"dir",
"el",
"faya",
"fi",
"fir",
"ha",
"hab",
"ia",
"idj",
"ir",
"is",
"ja",
"jel",
"ka",
"kah",
"kha",
"khari",
"la",
"lah",
"ma",
"na",
"nen",
"ra",
"ran",
"rar",
"rata",
"rin",
"rem",
"run",
"sef",
"sumah",
"tar",
"ya",
"yan",
"yil"};

AString	aPrefViking[] = {
"al",
"ber",
"drammen",
"grong",
"hag",
"hauge",
"hed",
"kinsar",
"kol",
"koper",
"lin",
"nas",
"norr",
"olof",
"os",
"Ost",
"Oster",
"skellef",
"soder",
"stal",
"stavan",
"stock",
"tons",
"trond",
"vin"};
AString	aSufViking[] = {
"fors",
"gard",
"heim",
"holm",
"lag",
"mar",
"marden",
"mark",
"stad",
"strom"};

AString	aPrefHumans[] = {
"basing",
"birming",
"black",
"bland",
"bletch",
"brack",
"brent",
"bridge",
"broms",
"bur",
"cam",
"canter",
"chelten",
"chester",
"col",
"dor",
"dun",
"glaston",
"grim",
"grin",
"harro",
"hastle",
"hels",
"hemp",
"herne",
"horn",
"hors",
"hum",
"ketter",
"lei",
"maiden",
"marble",
"mar",
"mel",
"new",
"nor",
"notting",
"oak",
"ox",
"ports",
"sher",
"stam",
"stan",
"stock",
"stroud",
"tuan",
"warring",
"wind"};
AString	aSufHumans[] = {
"dare",
"don",
"field",
"ford",
"grove",
"ham",
"hill",
"lock",
"mere",
"moor",
"ton",
"vil",
"wood"};

/*
Aerie of the Eagles
Ales 'n Beds
Alicorn, The
Baalzepub
Bag and Flagon, The
Banjo and Zigzag, The
Barf Bag, The
Basque and Feijoa, The
Battle Axe Inn
Behir and the Beholder, The
Bent Elbows, The
Black Boar, The
Black Hole, The
Black Pearl, The (near the ocean)
Black Stallion Tavern, The
Black Swan, The (sounds like a posh joint on a calm pond to me)
Blind Basilisk, The
Blood of the Vine (from Ravenloft I believe)
Blue Duck, The
Blue Sky Inn, The
Boar's Bristles, The
Boar's Chase, The
Boar's Head Inn
Boar's Rest
Bob's Place
Boil and Youth, The
Bowl's Rim, The
Brass Bucket Tavern, The
Broken Cane, The
Broken Drum, The
Broken Fist, The
Broken Kender Arms, The (with good reason...)
Bullseye Tavern, The
Bunch o' Grapes
Buxom Wench, The
Candi Bar, The (yet another fine erotic entertainment center)
Cat and Mouse, The
Cat's Claw Inn, The
Cesspit, The
Chicken's Rest, The
Chipped Chalice
Citadel 
Clan of the White Wind
Cloven Hoof, The
Clowns, Inn of
Cock and Hen, The
Common Folks Tavern, The
Copper Cup Tavern, The
Cork 'n' Bottle Tavern
Crippled Griffon
Crossed Daggers, The
Crossed Swords, The
Crow's Nest, The
Dawn Tavern, The
Days End Inn
Dead's End Tavern
Derrin's Divine Brews
Dirty Dog, The
Dirty Dwarf, The
Dirty Laundry, The
Divine Delights
Dog and Thrush
Dragon Skulls, The (with a pair of same over the bar)
Dragon's Breath, The
Dragon's Butt Tavern
Dragon's Hoarde, The
Dragon's Lair
Dragon's Spittoon
Drunken Boar, The
Drunken Cavalier, The
Drunken Cow , The
Drunken Sailor, The
Dustbin of Death, The
Eastern Delights, Inn of the
Efreeti's Wish, The
Elswers' (from a book called The World is Round" by Tony Rothman)
Ent Tent, The
Erics Erotic Dancers
Ever-full Jugs, The Tavern of
Eye of the Specter
Farlanghn's Rest
Fat Jack's Blues & Grub
Fawning Orc, The
Feetal's Gizzard
Fig and Gristle, The
Fisherman's Friend, The
Flame, The
Flaming Snake, The
Foresthome Rest
Forsaken Souls, Inn of
Fox and Hound, The
Fox Hole, The
Fox's Tail, The
Friendly House Inn, The
Friends of the Queen
Frog and Duck
Frog and Peach
Frosty Mug, The
Frozen Axe, The
Full Belly Inn, The
Full Moon Tavern, The
Full Tankard, The
Gaff and Slasher, The
Geezul's Place
Gelded Ranger, The
Giant's Nose, The
Gnoll's Belly, The
Golden Beer Tavern, The
Golden Chain, The
Golden Dragon Inn, The
Golden Gryphon, The
Golden Lady, The
Golden Perch
Golden Schooner, The
Golden Stool, The
Good Rest Inn
Gory's Tavern
Grapes of Wrath Tavern
Gray Wizard, The
Greasy Squeal, The
Great Arm, The Inn of the
Green Dragon, The
Green Griffon, The
Green Manalishi Inn, The
Grobo's Grotto
Grumbling Dwarf, The
Halflings' Revenge
Half-way Inn, The
Hammer, Anvil, and Stirrup, The
Happy Harpy Tavern, The
Happy Hooker, The
Happy Saturday's
Harpy's Hideaway, The
Haudhla's
Head On Inn
Heart of Oak, The
Hearthstone Arms
Hector's Nectar Tavern
Helix, The
Hellish Day Inn
Here's Sin in Your Eye
Highlighted Eye, Inn of the
Hillshire Farm
Hobbit's Armpit, The
Honest Barrister, The
Horn and Haunch, The
Horn and Hounds, The
Horny Halfling, The
Horwrath's Haven
Ice Dragon House
Imps Intestine, The
Inn of Clowns
Inn, The
Inter-planetary House of Spam
Iron Fist, The
Ivy Bush
Jade Terrorist, The
Joe's
Joker's Maniacal Frogleg and Nosewart Cafe, The
Karnov's Bar and Grill
Kettles Inn
Khaki Garter, The
Kibbles n' Bits' Inn
King's Bunk Inn
King's Coin, The
King's Rest, The
Knife in the Back, The
Knight's Tournament, The
Knyte Lyfe (owners name is Knyte)
Kraken, The
Kraun's
Labyrinth, The
Lady Russet Tavern, The
Lascivious Fairy, The
Last Port Tavern
Laughing Cow
Laurence's Mug-house
Lecherous Virgin, The
Lion's Den, The
Lonely Haystack, The
Lonely Wanderer, The Inn of the
Looter's (with barmaids that are thieves)
Lost Resort, The
Lotus, The
Lynched Orc Tavern, The
Maggot's Rest
Manicured Mouse, The
Master and Aubergine, The
Mended Drum, The
Mermaid's Smile, The
Momma's
Monk and Eggplant, The
Mousehole, The
Murdered Priest, The
Musty Moldan's Mighty Morsels
Nag's Head, The
Nanny's Arms, The
Noisy Bed Inn, The
Northern Road, Inn of the
Octopus Club, The
Old Grant's Ale-house
Old Lantern, The
Old Man and the Smee, The
Old Rubin's
Olde Fox Decieved, The
One-legged Man, The
Only Bar in the Multiverse Without a Convenient Back Door, The
Only Port in a Storm Inn, The
Open Book, The (caters to sages, students, etc.)
Orange Ogre, The
Orc's Armpit
Orc's Head Inn
Organ and Poinsettia, The
Perfumed 'Possum, The
Pink Dragon, The
Pink Lady Inn
Pipe and Tobacco, The
Plastered Imbecile, The
Pleasure Palace, The
Pot Belly, The
Prancing Pony, The (Tolkien of course)
Puking Unicorn, The
Puzzle and Egg, The
Quene's Hed, The
Quiet Night Inn, The
Quincy's
Raging Flea, The
Rain Song, The
Rampaging Rabbit, The
Rampant Dragon, The
Ram's Lord's Tavern
Ranger's Lookout, The
Rasvi & Vijay's Stop and Eat
Razorback Inn, The
Red Lantern, The
Red Snake Tavern, The
Roamer's Pillow, The
Rock Rock Rock (a dwarf-run tavern which loses its' meaning in the translation to common Pratchett?)
Rolling Meadows, The
Rose Gem, Inn of the
Rotgut Room, The
Ruby Blade, The
Ruby Lips, The
Sahuagen and Lady, The
Satyr and Stiletto, The
Scheister's Casino, Inn, and Mortuary (all in one)
Sea Barrel
Seabreeze Inn
Sexist Pig, The
Shining Gem, The
Short Neck Clam Town Inn and Tavern
Shot in the Dark, The
Sign of the Cringing Eunuch, The
Sign of the Golden Orange, The
Sign of the Jester, The
Sign of the Sign (the sign out front has a recursive painting of a sign on a sign on a sign...)
Sign of the Talon, The
Silver Axe, The
Silver Oak Inn, The
Silver Token Inn, The
Singing Boar, The
Site of the Harvest
Six Flags over Mead
Slade's
Sleeping Demon, The
Slugfest, The
Snake Pit, The
Snoring Flea Inn, The
Spend the Knight Inn
Splintered Ego Tavern
Spread-eagled Bugbear, The
Squalid Claw, The
Stinking Boar Inn, The
Stinking Cow, The
Strange Brew
Strangled Cat, The
Strong Drink
Stumpy Bob's House of Beer
Succubus, The
Sunset Inn, The
Svirfneblin's Ear, The
Swashbuckler's Luck
Sword's Sleep
Tavern of Ever-full Jugs, The
Tavern on the Edge (of something )
The Beardless Dwarf Inn
The Burning Spirit Tavern
The Cozy Pillow Inn
The Flickering Candle Inn
The Merry Tavern
The Sleeping Booty Inn
Thiefs Knot, The
Thirsty Whale, The
Three Gables, The
Thug and Succubus, The
Thunderbird Club, The
Thurston's Brass Tankard
Tipsy Tarrasque, The
Toss-It-Back, The (patrons throw mugs back at bartenders, who have gloves of missile catching)
Tower Tavern, The
Trog Scent Tavern
Trough, The (serves BIG drinks)
Turkey Cove Sleep and Eat
Two Halfling's Inn, The
Two-Fingered Troll, The
Ulcer, The
Valabar's
Van Smack's Tavern and Inn
Victorious Egret, The
Vincent's Gin-Palace
Violet Vein, The
Virgin and Nightmare, The
Voluptuous Elf, The
Wandering Mistrel, The
Wanton Wench, The
Watchman's Light, The
Wayfarer's Home, The
Wayfarer's Lay, The
Weigh Inn, The
Well Worn Wench, The Inn of the
We're Not Hostel
Whisper and Boil, The
Whistling Pig, The
White Hand, The
White Sheet Inn, The
White Tree, The
Wig and Thistle, The
Wild Chessman, The
Wine of Heaven, The
Wistful Wyvern, The
Wizard's Tower, The
Wolfhound, The
Wolf's Den (cliche),
Woolystonecrafters Inn, The
World's End Inn
Yawing Portal, The (Waterdeep entrance to Undermountain)
Yellow Necromancer, The
Zoo, The

*/


AString	aPrefInn[] = {
"Bent",
"Black",
"Blind",
"Blue",
"Bob's",
"Joe's",
"Broken",
"Buxom",
"Cat's",
"Crow's",
"Dirty",
"Dragon",
"Dragon's",
"Drunken",
"Diamond",
"Eagle's",
"Eastern",
"Falcon's",
"Fawning",
"Fiend's",
"Flaming",
"Frosty",
"Frozen",
"Gilded",
"Genie's",
"Golden",
"Golden",
"Gray",
"Green",
"King's",
"Licked",
"Lion's",
"Iron",
"Mended",
"Octopus",
"Old",
"Old",
"Orc's",
"Pink",
"Pot",
"Puking",
"Queen's",
"Red",
"Ruby",
"Delicate",
"Sea",
"Sexy",
"Shining",
"Silver",
"Singing",
"Steel",
"Strange",
"Thirsty",
"Violet",
"White",
"Wild",
"Yawing"};
AString	aSufInn[] = {
" Axe",
" Anchor",
" Barrel",
" Basilisk",
" Belly",
" Blade",
" Boar",
" Breath",
" Brew",
" Claw",
" Coin",
" Delight",
" Den",
" Dragon",
" Drum",
" Dwarf",
" Fist",
" Flower",
" Gem",
" Gryphon",
" Hand",
" Head",
" Hole",
" Inn",
" Lady",
" Maiden",
" Lantern",
" Monk",
" Mug",
" Nest",
" Orc",
" Paradise",
" Pearl",
" Pig",
" Pit",
" Place",
" Tavern",
" Portal",
" Ranger",
" Rest",
" Sailor",
" Sleep",
" Song",
" Swan",
" Swords",
" Tree",
" Unicorn",
" Whale",
" Wish",
" Wizard",
" Rain"};


AString	aPrefFort[] = {
"Mind ",
"Iron ",
"Demention ",
"Demonic ",
"Blood ",
"Mistery ",
"Ancient ",
"Doom",
"Black ",
"Crimson ",
"Blue ",
"Eternal ",
"Cursed ",
"Funny ",
"Stone",
"Etherial ",
"Phantom ",
"Forgotten ",
"King's ",
"Queen's ",
"Royal ",
"Fallen",
"Lost ",
"Warrior's ",
"Sorcerer's ",
"Steel ",
"Blademaster's ",
"Screaming ",
"Ice ",
"Frozen ",
"Dragon ",
"Glorious ",
"Infernal "};
AString	aSufFort[] = {
"Storm",
"Fist",
"Keep",
"Rage",
"Rose",
"Residence",
"Mansion",
"Haven",
"Gates",
""
};

AString	aPrefShip[] = {
"Absolute",
"Adventure",
"Alisa",
"Altered",
"Amber",
"Ancient",
"Angel's",
"Animal",
"Another",
"Azure",
"Bad",
"Bad Moon",
"Betty",
"Big",
"Black",
"Blue",
"Breaking",
"Crime",
"Crimson",
"Dancing",
"Dark",
"Dawn",
"Dirty",
"Distant",
"Double",
"Dragon",
"Dream",
"Emerald",
"Empty",
"Enchanted",
"Exotic",
"Extra",
"Extreme",
"Fallen",
"Fast",
"Fatal",
"Fifth",
"Final",
"Fine",
"Fire",
"First",
"Flying",
"Foreign",
"Fortune",
"Funny",
"Gentle",
"Golden",
"Grand",
"Great",
"Green",
"Grey",
"Gypsy",
"Half",
"Happy",
"High",
"Impossible",
"Jade",
"Little",
"Lone",
"Lucky",
"Mad",
"Mermaid",
"Midnight",
"Moon",
"Morning",
"Naked",
"Naughty",
"Naval",
"New",
"Night",
"Ocean",
"Old",
"Pacific",
"Perfect",
"Pretty",
"Quick",
"Quiet",
"Red",
"Saint",
"Sea",
"Sapphire",
"Second",
"Silver",
"Southern",
"Stella",
"Sun",
"Sunset",
"Sweet",
"Third",
"Thunder",
"Treasure",
"Ultimate",
"Wave",
"Zephyr",
"Zodiac"
};
AString	aSufShip[] = {
" Adventure",
" Amore",
" Angel",
" Answer",
" Attraction",
" Bird",
" Boat",
" Body",
" Bound",
" Boy",
" Breaker",
" Breeze",
" Cat",
" Catcher",
" Chaser",
" Courier",
" Crusher",
" Devil",
" Diamond",
" Dog",
" Dolphin",
" Dream",
" Dreamer",
" Eagle",
" Elf",
" Fish",
" Flash",
" Flight",
" Fox",
" Girl",
" Ghost",
" Goose",
" Gull",
" Hawk",
" Huntress",
" Hunter",
" Jack",
" Jane",
" Jewel",
" Jumper",
" Karma",
" King",
" Kiss",
" Knight",
" Lady",
" Lion",
" Love",
" Lover",
" Madness",
" Magic",
" Marie",
" Minstrel",
" Mist",
" Mistake",
" Money",
" Monkey",
" Monster",
" Nest",
" Nightmare",
" Owl",
" Queen",
" Quest",
" Pig",
" Pirate",
" Plainsman",
" Phantom",
" Power",
" Presence",
" Prince",
" Princess",
" Rainbow",
" Rising",
" Rose",
" Runner",
" Scare",
" Seeker",
" Sight",
" Sirena",
" Sixteen",
" Shadow",
" Shift",
" Shine",
" Stalker",
" Stripe",
" Song",
" Spirit",
" Spice",
" Star",
" Storm",
" Swan",
" Tide",
" Tiger",
" Toy",
" Trouble",
" Turtle",
" Viking",
" Unicorn",
" Walker",
" Wind",
" Wine",
" Wish",
" Witch",
" Wizard",
" White",
" Wolf",
" Woman",
" Zebra"
};

AString	aPrefFemale[] = {
"Ail",
"Ara",
"Ay",
"Bren",
"Astar",
"Dae",
"Dren",
"Dwen",
"El",
"Erin",
"Eth",
"Fae",
"Fay",
"Gae",
"Gay",
"Glae",
"Gwen",
"Il",
"Jey",
"Lae",
"Lan",
"Lin",
"Mae",
"Mara",
"More",
"Mi",
"Min",
"Ne",
"Nel",
"Pae",
"Pwen",
"Rae",
"Ray",
"Re",
"Ri",
"Si",
"Sal",
"Say",
"Tae",
"Te",
"Ti",
"Tin",
"Tir",
"Vi",
"Vul"
};
AString	aSufFemale[] = {
"ta",
"alle",
"ann",
"arra",
"aye",
"da",
"dolen",
"ell",
"enn",
"eth",
"eya",
"fa",
"fey",
"ga",
"gwenn",
"hild",
"ill",
"ith",
"la",
"lana",
"lar",
"len",
"lwen",
"ma",
"may",
"na",
"narra",
"navia",
"nwen",
"ola",
"pera",
"pinn",
"ra",
"rann",
"rell",
"ress",
"reth",
"riss",
"sa",
"shann",
"shara",
"shea",
"shell",
"tarra",
"tey",
"ty",
"unn",
"ura",
"valia",
"vara",
"vinn",
"wen",
"weth",
"wynn",
"wyrr",
"ya",
"ye",
"yll",
"ynd",
"yrr",
"yth"
};

AString	aPrefMale[] = {
"ache",
"aim",
"bald",
"bear",
"cron",
"boar",
"boast",
"boil",
"boni",
"boy",
"bower",
"churl",
"corn",
"cuff",
"dark",
"dire",
"dour",
"dross",
"dupe",
"dusk",
"dwar",
"dwarf",
"ebb",
"el",
"elf",
"fag",
"fate",
"fay",
"fell",
"fly",
"fowl",
"gard",
"gay",
"gilt",
"girth",
"glut",
"goad",
"gold",
"gorge",
"grey",
"groan",
"haft",
"hale",
"hawk",
"haught",
"hiss",
"hock",
"hoof",
"hook",
"horn",
"kin",
"kith",
"lank",
"leaf",
"lewd",
"louse",
"lure",
"man",
"mars",
"meed",
"moat",
"mould",
"muff",
"muse",
"not",
"numb",
"odd",
"ooze",
"ox",
"pale",
"port",
"quid",
"rau",
"red",
"rich",
"rob",
"rod",
"rud",
"ruff",
"run",
"rush",
"scoff",
"skew",
"sky",
"sly",
"sow",
"stave",
"steed",
"swar",
"thor",
"tort",
"twig",
"twit",
"vain",
"vent",
"vile",
"wail",
"war",
"whip",
"wise",
"worm",
"yip"
};
AString	aSufMale[] = {
"os",
"ard",
"bald",
"ban",
"baugh",
"bert",
"brand",
"cas",
"celot",
"cent",
"cester",
"cott",
"dane",
"dard",
"doch",
"dolph",
"don",
"doric",
"dower",
"dred",
"fird",
"ford",
"fram",
"fred",
"frid",
"fried",
"gal",
"gard",
"gernon",
"gill",
"gurd",
"gus",
"ham",
"hard",
"hart",
"helm",
"horne",
"ister",
"kild",
"lan",
"lard",
"ley",
"lisle",
"loch",
"man",
"mar",
"mas",
"mon",
"mond",
"mour",
"mund",
"nald",
"nard",
"nath",
"ney",
"olas",
"pold",
"rad",
"ram",
"rard",
"red",
"rence",
"reth",
"rick",
"ridge",
"riel",
"ron",
"rone",
"roth",
"sander",
"sard",
"shall",
"shaw",
"son",
"steen",
"stone",
"ter",
"than",
"ther",
"thon",
"thur",
"ton",
"tor",
"tran",
"tus",
"ulf",
"vald",
"van",
"vard",
"ven",
"vid",
"vred",
"wald",
"wallader",
"ward",
"werth",
"wig",
"win",
"wood",
"yard"
};

AString	aEpithetMageM[] = {
"Black",
"White",
"Blue",
"Green",
"Brown",
"Ruthless",
"Heartless",
"Ugly",
"Mad",
"Careless",
"Restless",
"Impartial",
"Immortal",
"Colourless",
"Callous",
"Cruel",
"Powerful",
"Mage",
"Evil",
"Weak",
"Wize",
"Handless",
"Furious",
"Flamer",
"Malicius"
};
AString	aEpithetMageF[] = {
"Black",
"White",
"Blue",
"Green",
"Brown",
"Ruthless",
"Heartless",
"Ugly",
"Mad",
"Careless",
"Restless",
"Impartial",
"Immortal",
"Colourless",
"Callous",
"Cruel",
"Powerful",
"Witch",
"Evil",
"Weak",
"Wize",
"Handless",
"Furious",
"Malicius"
};
AString	aEpithetClericM[] = {
"Good",
"Merciful",
"Compassionate",
"Healer",
"Magnanimous",
"Saint",
"High Priest",
"Hermit",
"Monk",
"Brother",
"Father",
"Cleric",
"Wizard",
"Elder",
"Deathkiller",
"Painkiller",
"Reviver",
"Accurate",
"Silver",
"Snake",
"Silent",
"Quiet",
"Kindest",
"Childless",
"Herbologist",
"Alchemist",
"Modest"
};
AString	aEpithetClericF[] = {
"Good",
"Merciful",
"Compassionate",
"Healer",
"Magnanimous",
"Saint",
"High Priestess",
"Cleric",
"Sorceress",
"Sister",
"Mother",
"Elder",
"Accurate",
"Silver",
"Beautiful",
"Silent",
"Quiet",
"Kindest",
"Childless",
"Herbologist",
"Alchemist"
};
AString	aEpithetShamanM[] = {
"Summoner",
"Shaman",
"Chanter",
"Scull",
"Claw",
"Warlock",
"Dragon",
"Elder",
"Incomer",
"Dominator",
"Beastlord",
"Master",
"Outsider",
"Exiled",
"Halfhuman",
"Ancient",
"Returned",
"Banisher",
"Lonely",
"Seeker",
"Wereman"
};
AString	aEpithetShamanF[] = {
"Dominatrix",
"Mistress",
"Black Widow",
"Ancient",
"Lonely",
"Werewoman"
};
AString	aEpithetGeneralM[] = {
"Ruthless",
"Bloody",
"Mighty",
"Wild",
//"Demigod",
"Lord",
"Headcutter",
"Executor",
"Loud",
"Prince",
"King",
"Don",
"Commander",
"Great",
"Scullsmasher",
"Big Axe",
"Heavy Axe",
"Heavy Fist",
"Quick Sword",
"Golden Sword",
"Silver Sword",
"Iron Sword",
"Iron Fist",
"Wooden Sword",
"Sharp Sword",
"Deadly Blade",
"Shining Sword",
"Black Sword",
"First Sword",
"Champion",
"Big Mug",
"Demon Hunter",
"Big Helm",
"Horny",
"Dragonslayer",
"Peacemaker",
"Liberator",
"Barbarian",
"Dark Knight",
"White Knight",
"Glorious",
"One-eye",
"Butcher",
"Murderer",
"Capitan",
"General",
"Warlord",
"Chief"
};
AString	aEpithetGeneralF[] = {
"Ruthless",
"Bloody",
"Mighty",
"Wild",
//"Demigoddess",
"Executor",
"Princess",
"Queen",
"Great",
"Lady",
"Quick Sword",
"Golden Sword",
"Silver Sword",
"Iron Sword",
"Wooden Sword",
"Sharp Sword",
"Deadly Blade",
"Shining Sword",
"Black Sword",
"Demon Hunter",
"Dragonslayer",
"Glorious",
"Capitan",
"General",
"Warlord",
"Chief"
};
//---------------------------------------------------------------------------

AString GetEthnicPrefix(int iRace, int iSeed)
{
    iSeed = abs(iSeed);
	AString first;
	int w,h,mul;
	switch(iRace)
	{
		case MAN_VIKING:
				w = sizeof(aPrefViking)/sizeof(AString);
				h = sizeof(aSufViking)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefViking[mul%w];
			break;
		case MAN_HIGHLANDER:
		case MAN_BARBARIAN:
				w = sizeof(aPrefScotish)/sizeof(AString);
				h = sizeof(aSufScotish)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefScotish[mul%w];
			break;
		case MAN_PLAINSMAN:
		case MAN_MAN:
		case MAN_MERC:
		case MAN_HOBBIT:
				w = sizeof(aPrefHumans)/sizeof(AString);
				h = sizeof(aSufHumans)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefHumans[mul%w];
			break;
		case MAN_ESKIMO:
				w = sizeof(aPrefEscimo)/sizeof(AString);
				h = sizeof(aSufEscimo)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefEscimo[mul%w];
			break;
		case MAN_NOMAD:
				w = sizeof(aPrefArabic)/sizeof(AString);
				h = sizeof(aSufArabic)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefArabic[mul%w];
			break;
		case MAN_TRIBESMAN:
		case MAN_AMAZON:
		case MAN_DARKMAN:
				w = sizeof(aPrefAfrica)/sizeof(AString);
				h = sizeof(aSufAfrica)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefAfrica[mul%w];
			break;
		case MAN_HIGHELF:
		case MAN_FAIRY:
				w = sizeof(aPrefElven1)/sizeof(AString);
				h = sizeof(aSufElven1)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefElven1[mul%w];
			break;
		case MAN_WOODELF:
		case MAN_SEAELF:
		case MAN_TRIBALELF:
				w = sizeof(aPrefElven2)/sizeof(AString);
				h = sizeof(aSufElven2)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefElven2[mul%w];
			break;
		case MAN_ICEDWARF:
		case MAN_HILLDWARF:
		case MAN_UNDERDWARF:
		case MAN_DESERTDWARF:
		case MAN_GNOME:
				w = sizeof(aPrefDwarven)/sizeof(AString);
				h = sizeof(aSufDwarven)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefDwarven[mul%w];
			break;
		case MAN_ORC:
		case MAN_URUK:
		case MAN_GOBLIN:
		case MAN_OGRE:
				w = sizeof(aPrefOrchish)/sizeof(AString);
				h = sizeof(aSufOrchish)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefOrchish[mul%w];
			break;
		case MAN_LIZARDMAN:
		case MAN_MINOTAUR:
		case MAN_GNOLL:
				w = sizeof(aPrefGreek)/sizeof(AString);
				h = sizeof(aSufGreek)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefGreek[mul%w];
			break;
		case MAN_DROW:
		case MAN_GREYELF:
				w = sizeof(aPrefDrow)/sizeof(AString);
				h = sizeof(aSufDrow)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefDrow[mul%w];
			break;
		case MAN_TITAN:
				w = sizeof(aPrefAztec)/sizeof(AString);
				h = sizeof(aSufAztec)/sizeof(AString);
				mul = iSeed%(w*h);
				first = aPrefAztec[mul%w];
			break;
		default:
				w = sizeof(aPrefMale)/sizeof(AString);
				mul = iSeed%(w);
				first = aPrefMale[mul%w];
	}

	return first;
}

AString GetEthnicSuffix(int iRace, int iSeed)
{
    iSeed = abs(iSeed);
	AString second;
	int w,h,mul;
	switch(iRace)
	{
		case MAN_VIKING:
				w = sizeof(aPrefViking)/sizeof(AString);
				h = sizeof(aSufViking)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufViking[mul/w];
			break;
		case MAN_HIGHLANDER:
		case MAN_BARBARIAN:
				w = sizeof(aPrefScotish)/sizeof(AString);
				h = sizeof(aSufScotish)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufScotish[mul/w];
			break;
		case MAN_PLAINSMAN:
		case MAN_MAN:
		case MAN_MERC:
		case MAN_HOBBIT:
				w = sizeof(aPrefHumans)/sizeof(AString);
				h = sizeof(aSufHumans)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufHumans[mul/w];
			break;
		case MAN_ESKIMO:
				w = sizeof(aPrefEscimo)/sizeof(AString);
				h = sizeof(aSufEscimo)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufEscimo[mul/w];
			break;
		case MAN_NOMAD:
				w = sizeof(aPrefArabic)/sizeof(AString);
				h = sizeof(aSufArabic)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufArabic[mul/w];
			break;
		case MAN_TRIBESMAN:
		case MAN_AMAZON:
		case MAN_DARKMAN:
				w = sizeof(aPrefAfrica)/sizeof(AString);
				h = sizeof(aSufAfrica)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufAfrica[mul/w];
			break;
		case MAN_HIGHELF:
		case MAN_FAIRY:
				w = sizeof(aPrefElven1)/sizeof(AString);
				h = sizeof(aSufElven1)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufElven1[mul/w];
			break;
		case MAN_WOODELF:
		case MAN_SEAELF:
		case MAN_TRIBALELF:
				w = sizeof(aPrefElven2)/sizeof(AString);
				h = sizeof(aSufElven2)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufElven2[mul/w];
			break;
		case MAN_ICEDWARF:
		case MAN_HILLDWARF:
		case MAN_UNDERDWARF:
		case MAN_DESERTDWARF:
		case MAN_GNOME:
				w = sizeof(aPrefDwarven)/sizeof(AString);
				h = sizeof(aSufDwarven)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufDwarven[mul/w];
			break;
		case MAN_ORC:
		case MAN_URUK:
		case MAN_GOBLIN:
		case MAN_OGRE:
				w = sizeof(aPrefOrchish)/sizeof(AString);
				h = sizeof(aSufOrchish)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufOrchish[mul/w];
			break;
		case MAN_LIZARDMAN:
		case MAN_MINOTAUR:
		case MAN_GNOLL:
				w = sizeof(aPrefGreek)/sizeof(AString);
				h = sizeof(aSufGreek)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufGreek[mul/w];
			break;
		case MAN_DROW:
		case MAN_GREYELF:
				w = sizeof(aPrefDrow)/sizeof(AString);
				h = sizeof(aSufDrow)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufDrow[mul/w];
			break;
		case MAN_TITAN:
				w = sizeof(aPrefAztec)/sizeof(AString);
				h = sizeof(aSufAztec)/sizeof(AString);
				mul = iSeed%(w*h);
				second = aSufAztec[mul/w];
			break;
		default:
				w = sizeof(aSufMale)/sizeof(AString);
				mul = iSeed%(w);
				second = aSufMale[mul/w];
	}

	return second;
}

AString GetHeroName(int iSeed, char *classname, int iRace)
{
    iSeed = abs(iSeed);
	AString first, second;
	int w,h,mul;

	int male;
	male = iSeed%2;

	if(classname == 0)
	{
		int cls = iSeed%4;
		switch(cls)
		{
			case 0: classname = "$Shaman";
				break;
			case 1: classname = "$Cleric";
				break;
			case 2: classname = "$Mage";
				break;
			case 3: classname = "$General";
				break;
		}
	}

	w = 1;
	if(male==1)
	{
//		w = sizeof(aPrefMale)/sizeof(AString);
		h = sizeof(aSufMale)/sizeof(AString);
//		mul = iSeed%(w*h);
		mul = iSeed%(h);
//		first = aPrefMale[mul%w];
		first = GetEthnicPrefix(iRace, iSeed);
		second = aSufMale[mul/w];
	}
	else
	{
//		w = sizeof(aPrefFemale)/sizeof(AString);
		h = sizeof(aSufFemale)/sizeof(AString);
//		mul = iSeed%(w*h);
		mul = iSeed%(h);
//		first = aPrefFemale[mul%w];
		first = GetEthnicPrefix(iRace, iSeed);
		second = aSufFemale[mul/w];
	}

	char t;
	t = first.Str()[0];
	if(t >= 'a' && t <= 'z')
	{
		t = t - 'a' + 'A';
		first.Str()[0] = t;
	}

	AString ts;
	AString epithet = "";
	ts = AString(classname);

	if(ts == "$Shaman")
	{
		if(male == 1)
		{
			mul = iSeed%(sizeof(aEpithetShamanM)/sizeof(AString));
			epithet = aEpithetShamanM[mul];
		}
		else
		{
			mul = iSeed%(sizeof(aEpithetShamanF)/sizeof(AString));
			epithet = aEpithetShamanF[mul];
		}
	}
	if(ts == "$Cleric")
	{
		if(male == 1)
		{
			mul = iSeed%(sizeof(aEpithetClericM)/sizeof(AString));
			epithet = aEpithetClericM[mul];
		}
		else
		{
			mul = iSeed%(sizeof(aEpithetClericF)/sizeof(AString));
			epithet = aEpithetClericF[mul];
		}
	}
	if(ts == "$Mage")
	{
		if(male == 1)
		{
			mul = iSeed%(sizeof(aEpithetMageM)/sizeof(AString));
			epithet = aEpithetMageM[mul];
		}
		else
		{
			mul = iSeed%(sizeof(aEpithetMageF)/sizeof(AString));
			epithet = aEpithetMageF[mul];
		}
	}
	if(ts == "$General")
	{
		if(male == 1)
		{
			mul = iSeed%(sizeof(aEpithetGeneralM)/sizeof(AString));
			epithet = aEpithetGeneralM[mul];
		}
		else
		{
			mul = iSeed%(sizeof(aEpithetGeneralF)/sizeof(AString));
			epithet = aEpithetGeneralF[mul];
		}
	}

	if(iSeed == 4)
	{
		if(male == 1)
		{
			epithet = "Demigod";
		}
		else
		{
			epithet = "Demigoddess";
		}
	}

	if(iSeed%2 != 1 && abs(iSeed*iSeed)%5 < 2)// && !(epithet == ""))
	{
		second = second + " the " + epithet;
	}
	else
	{
		first = epithet + " " + first;
	}

	return first+second;
}

AString GetAbstractName(int iSeed)
{
    iSeed = abs(iSeed);
	AString first, second;
	int w,h,mul;

	w = sizeof(aPrefAbstract)/sizeof(AString);
	h = sizeof(aSufAbstract)/sizeof(AString);
	mul = iSeed%(w*h);
	first = aPrefAbstract[mul%w];
	second = aSufAbstract[mul/w];

	char t;
	t = first.Str()[0];
	if(t >= 'a' && t <= 'z')
	{
		t = t - 'a' + 'A';
		first.Str()[0] = t;
	}

	return first+second;
}

AString GetShipName(int iSeed)
{
    iSeed = abs(iSeed);
	AString first, second;
	int w,h,mul;

	w = sizeof(aPrefShip)/sizeof(AString);
	h = sizeof(aSufShip)/sizeof(AString);
	mul = iSeed%(w*h);
	first = aPrefShip[mul%w];
	second = aSufShip[mul/w];

	char t;
	t = first.Str()[0];
	if(t >= 'a' && t <= 'z')
	{
		t = t - 'a' + 'A';
		first.Str()[0] = t;
	}

	return first+second;
}

AString GetFortressName(int iSeed, int iFort)
{
    iSeed = abs(iSeed);
	AString first, second;
	int w,h,mul;

	w = sizeof(aPrefFort)/sizeof(AString);
	h = sizeof(aSufFort)/sizeof(AString);
	mul = iSeed%(w*h);
	first = aPrefFort[mul%w];
	second = aSufFort[mul/w];

	if(second == "")
	{
		second = (AString)" " + ObjectDefs[iFort].name;
		first = *(first.gettoken());
//		first = (AString)(((AString)first.Str()).Trim().c_str());
	}

	char t;
	t = first.Str()[0];
	if(t >= 'a' && t <= 'z')
	{
		t = t - 'a' + 'A';
		first.Str()[0] = t;
	}

	return first+second;
}

AString GetInnName(int iSeed)
{
    iSeed = abs(iSeed);
	AString first, second;
	int w,h,mul;

	w = sizeof(aPrefInn)/sizeof(AString);
	h = sizeof(aSufInn)/sizeof(AString);
	mul = iSeed%(w*h);
	first = aPrefInn[mul%w];
	second = aSufInn[mul/w];

	char t;
	t = first.Str()[0];
	if(t >= 'a' && t <= 'z')
	{
		t = t - 'a' + 'A';
		first.Str()[0] = t;
	}

	return first+second;
}

AString GetObjectName(int iSeed, int iType)
{
	switch(iType)
	{
		case O_DUMMY:
			break;
		case O_LONGBOAT:
		case O_CLIPPER:
		case O_GALLEON:
		case O_BALLOON:
		case O_AGALLEON:
		case O_DERELICT:
		case O_MANOWAR:
		case O_WHALER:
		case O_CARAVELL:
		case O_BARGE:
		case O_BRIG:
		case O_AIRSIP:
		case O_ZEPPELIN:
		case O_FLYINGFORTRESS:
			return GetShipName(iSeed);
		case O_TOWER:
		case O_FORT:
		case O_KEEP:
		case O_CASTLE:
		case O_CITADEL:
		case O_MKEEP:
		case O_MCASTLE:
		case O_MCITADEL:
		case O_MTOWER:
		case O_PALACE:
		case O_STOCKADE:
		case O_CPALACE:
		case O_HTOWER:
		case O_MAGETOWER:
		case O_DARKTOWER:
		case O_GIANTCASTLE:
		case O_HPTOWER:
			return GetFortressName(iSeed, iType);
		case O_SHAFT:
		case O_LAIR:
		case O_RUIN:
		case O_CAVE:
		case O_DEMONPIT:
		case O_CRYPT:
		case O_MINE:
		case O_FARM:
		case O_RANCH:
		case O_TIMBERYARD:
		case O_QUARRY:
		case O_MQUARRY:
		case O_AMINE:
		case O_PRESERVE:
		case O_SACGROVE:
		case O_TRAPPINGHUT:
		case O_STABLE:
		case O_MSTABLE:
		case O_TRAPPINGLODGE:
		case O_FAERIERING:
		case O_ALCHEMISTLAB:
		case O_OASIS:
		case O_GEMAPPRAISER:
			break;
		case O_INN:
			return GetInnName(iSeed);
	// LLS
	// New ocean lairs
		case O_ISLE:
		case O_OCAVE:
		case O_WHIRL:
			break;
	// AS
		case O_ROADN:
		case O_ROADNW:
		case O_ROADNE:
		case O_ROADSW:
		case O_ROADSE:
		case O_ROADS:
			return "Ancient Road";
		// VVR
		case O_MROADN:
		case O_MROADNW:
		case O_MROADNE:
		case O_MROADSW:
		case O_MROADSE:
		case O_MROADS:
			return "Enchanted Ancient Road";
		//********
		case O_TEMPLE:
	// JT
	// Abyss Lair
		case O_BKEEP:
	// For Ceran
		case O_DCLIFFS:
		case O_HUT:
		case O_NGUILD:
		case O_AGUILD:
		case O_ATEMPLE:
	// Tzargs monster lairs
		case O_ILAIR:
		case O_ICECAVE:
		case O_BOG:
			break;
	}

	return ObjectDefs[iType].name;
}

AString GetEthnicName(int iRace, int iSeed)
{
	iSeed = abs(iSeed);
	if(iRace == -1)
	{
		return "";
	}

	AString first, second;

	first = GetEthnicPrefix(iRace, iSeed);
	second = GetEthnicSuffix(iRace, iSeed);

	char t;
	t = first.Str()[0];
	if(t >= 'a' && t <= 'z')
	{
		t = t - 'a' + 'A';
		first.Str()[0] = t;
	}

	return first+second;
}

AString GetForestName(AString sBase, int nameID, int area)
{
	if(nameID == -1)
		return "Nameless Forest";

	AString sResult;

	sResult = sBase;

	if(area<15)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Forest";
		else
			sResult = sResult + " Woods";
	}
	else
	{
		sResult = (AString)"Great " + sResult + " Forest";
	}
	return sResult;
}

AString GetJungleName(AString sBase, int nameID, int area)
{
	if(nameID == -1)
		return "Nameless Jungle";

	AString sResult;

	sResult = sBase;

	if(area<15)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Jungle";
		else
			sResult = sResult + " Woods";
	}
	else
	{
		sResult = (AString)"Great " + sResult + " Jungle";
	}
	return sResult;
}

AString GetDesertName(AString sBase, int nameID, int area)
{
	if(nameID == -1)
		return "Nameless Desert";

	AString sResult;

	sResult = sBase;

	if(area<15)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Desert";
		else
			sResult = sResult + " Sands";
	}
	else
	{
		sResult = (AString)"Great " + sResult + " Desert";
	}
	return sResult;
}

AString GetVolcanoName(AString sBase, int nameID, int area)
{
	if(nameID == -1)
		return "Nameless Volkano";

	AString sResult;

	sResult = sBase;

	if(nameID%2 == 1)
		sResult = sResult + " Volkano";
	else
		sResult = sResult + " Peak";

	return sResult;
}

AString GetMountainName(AString sBase, int nameID, int area)
{
	if(nameID == -1)
		return "Nameless Mountain";

	AString sResult;

	sResult = sBase;

	if(area==1)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Mountain";
		else
			sResult = sResult + " Peak";

		return sResult;
	}

	if(area<15)
	{
		if(nameID%3 == 1)
			sResult = sResult + " Mountains";
		else
		{
			if(nameID%2 == 1)
				sResult = sResult + " Heights";
			else
				sResult = sResult + " Rocks";
		}
		return sResult;
	}

	sResult = (AString)"Great " + sResult + " Mountains";
	return sResult;
}

AString GetHillsName(AString sBase, int nameID, int area)
{
	if(nameID == -1)
		return "Nameless Hills";

	AString sResult;

	sResult = sBase;

	if(area==1)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Hill";
		else
			sResult = sResult + " Barrow";

		return sResult;
	}

	if(area<15)
	{
		if(nameID%3 == 1)
			sResult = sResult + " Hills";
		else
		{
			if(nameID%2 == 1)
				sResult = sResult + " Heights";
			else
				sResult = sResult + " Barrows";
		}
		return sResult;
	}

	sResult = (AString)"Great " + sResult + " Hills";
	return sResult;
}

AString GetSwampName(AString sBase, int nameID, int area)
{
	if(nameID == -1)
		return "Nameless Swamp";

	AString sResult;

	sResult = sBase;

	if(area<15)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Swamp";
		else
			sResult = sResult + " Marshes";
	}
	else
	{
		sResult = (AString)"Great " + sResult + " Swamp";
	}
	return sResult;
}

AString GetPlainName(AString sBase, int nameID, int area)
{
	if(nameID == -1)
		return "Nameless Plain";

	AString sResult;

	sResult = sBase;

	if(area==1)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Dale";
		else
			sResult = sResult + " Plain";

		return sResult;
	}

	if(area<15)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Plains";
		else
			sResult = sResult + " Valley";

		return sResult;
	}

	sResult = (AString)"Great " + sResult + " Plains";
	return sResult;
}

AString GetTundraName(AString sBase, int nameID, int area)
{
	if(nameID == -1)
		return "Nameless Tundra";

	AString sResult;

	sResult = sBase;

	sResult = sResult + " Tundra";

	return sResult;
}

AString GetRegionName(int iType, int iSize, int island, int iRace, int iSeed)
{
	if(iSeed == -1)
	{
		return "yet unnamed";
	}

	AString sBase;

	sBase = GetEthnicName(iRace, iSeed*iSeed).Str();

	if(island)
	{
		return sBase + " Island";
	}

	switch(iType)
	{
		case R_FOREST:
		case R_UFOREST:
		case R_CERAN_FOREST1:
		case R_CERAN_FOREST2:
		case R_CERAN_FOREST3:
		case R_CERAN_MYSTFOREST:
		case R_CERAN_MYSTFOREST1:
		case R_CERAN_MYSTFOREST2:
		case R_CERAN_UFOREST1:
		case R_CERAN_UFOREST2:
		case R_CERAN_UFOREST3:
		case R_DFOREST:
		case R_CERAN_DFOREST1:
				return GetForestName(sBase, iSeed, iSize);
		case R_JUNGLE:
		case R_CERAN_JUNGLE1:
		case R_CERAN_JUNGLE2:
		case R_CERAN_JUNGLE3:
				return GetJungleName(sBase, iSeed, iSize);
		case R_DESERT:
		case R_CERAN_DESERT1:
		case R_CERAN_DESERT2:
		case R_CERAN_DESERT3:
				return GetDesertName(sBase, iSeed, iSize);
		case R_VOLCANO:
				return GetVolcanoName(sBase, iSeed, iSize);
		case R_MOUNTAIN:
		case R_ISLAND_MOUNTAIN:
		case R_CERAN_MOUNTAIN1:
		case R_CERAN_MOUNTAIN2:
		case R_CERAN_MOUNTAIN3:
		case R_HIGH_MOUNTAIN:
				return GetMountainName(sBase, iSeed, iSize);
		case R_CERAN_HILL:
		case R_CERAN_HILL1:
		case R_CERAN_HILL2:
		case R_HILLS:
				return GetHillsName(sBase, iSeed, iSize);
		case R_SWAMP:
		case R_ISLAND_SWAMP:
		case R_CERAN_SWAMP1:
		case R_CERAN_SWAMP2:
		case R_CERAN_SWAMP3:
				return GetSwampName(sBase, iSeed, iSize);
		case R_PLAIN:
		case R_ISLAND_PLAIN:
		case R_CERAN_PLAIN1:
		case R_CERAN_PLAIN2:
		case R_CERAN_PLAIN3:
				return GetPlainName(sBase, iSeed, iSize);
		case R_TUNDRA:
		case R_CERAN_TUNDRA1:
		case R_CERAN_TUNDRA2:
		case R_CERAN_TUNDRA3:
				return GetTundraName(sBase, iSeed, iSize);
		default:
				return (AString)GetAbstractName(iSeed*iSeed).Str();
	}
}


