#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define LCD_CS A3   // Chip Select goes to Analog 3
#define LCD_CD A2  // Command/Data goes to Analog 2
#define LCD_WR A1  // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define WIDTH 317
#define HEIGHT 477
#define SONG 198
#define BLACK  0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

MCUFRIEND_kbv tft;
SoftwareSerial mySerial(51, 53);
DFPlayerMini_Fast mp3;
byte vol = 0;
byte curVol = 0;
byte thickness;
byte musicIndex = 0;
byte curMusicIndex = 0;
byte musicList[169];
byte x_gap = 100;
byte y_gap = 200;
uint8_t EQ_ = 0;			// 0->normal 1->pop 2->rock 3->jazz 4->classic 5->base
byte optionIndex = 0;	// 0-> repeat 1-> shuffle 2->EQ  
bool buttonPushed;
bool paused = false;
bool option = false;
bool shuffleOn = false;
bool repeatOn = false;
bool buttonOn = false;		//prevent glitch
bool buttonOnChecker = false;
bool lock = false;
bool mp3On = false;
unsigned long stickHtimer = 0;
unsigned long stickVtimer = 0;
unsigned long buttonTimer = 0;
char* song_name[] = {//0~168
	"10cm - What the spring",
	"10cm - Phonecert",
	"Adam Levine - Lost Stars",
	"Bag Raiders - Shooting Stars",
	"BJJ - First Kiss",
	"Blonde - All Cried Out feat. Alex Newell                  (Oliver Nelson Remix)",
	"David Guetta - Play Hard",
	"DINY - Cosmic Fantastic Love song",
	"DJ Max - If",
	"DJ Max - Sweet Dream",
	"DJ Max Portable 2 - Your Own Miracle",
	"Dj max Portable 3 - Keys To The World",
	"Dj max Portable 3 - Trip",
	"DJ Max RESPECT - Only For You",
	"DJ Max Trilogy - Sunny Side",
	"DJMAX - Memory Of Beach",
	"Djmax Technika 3 - You & Me",
	"Dr. Dre - The Next Episode",
	"Enrique Iglesias - Do You Know",
	"Fall Out Boy - 27",
	"Fall Out Boy - A Little Less Sixteen                      Candles, A Little More Touch Me",
	"Fall Out Boy - Calm Before the Storm",
	"Fall Out Boy - Dead On Arrival",
	"Fall Out Boy - From Now On We Are Enemies",
	"Fall Out Boy - HOLD ME TIGHT OR DON'T",
	"Fall Out Boy - Hum Hallelujah",
	"Fall Out Boy - It's Not A Side Effect Of                 The Cocaine I Am Thinking It Must                    Be Love",
	"Fall Out Boy - Lake Effect Kid",
	"Fall Out Boy - Music or the Misery",
	"Fall Out Boy - Of All The Gin Joints                     In The W",
	"Fall Out Boy - The Carpal Tunnel of Love",
	"Fall Out Boy - The Pros and Cons                         of Breathing",
	"Fall Out Boy - Thnks Fr Th Mmrs",
	"Fall Out Boy - Thriller",
	"Fall Out Boy - w.a.m.s",
	"Fall Out Boy - West Coast Smoker",
	"Fall OutBoy - 7 Minutes In Heaven",
	"Gallant - Weight in Gold",
	"Gowe - Wait For You",
	"Hot Chelle Rae - I like it like that                       (feat. new boyz)",
	"Hot Chelle Rae - Recklessly",
	"Hot Chelle Rae - Tonight Tonight",
	"Jay-z - Dead presidents II",
	"Jay-Z - Encore",
	"Jay-Z - My 1st Song",
	"Jeremy Zucker - Comethru",
	"John Legend - P.D.A.                                      (We Just Don't Care)",
	"K.Will - Love119 (Feat. MC_Mong)",
	"Lin-G - I Want You",
	"Linkin Park - In the End",
	"Linkin Park - Lost In The Echo",
	"Loptimist - The Triumph                                   (Feat. Simon dominic)",
	"Macklemore & Ryan Lewis - Downtown",
	"Madeon & Passion Pit - Pay No Mind",
	"Madeon - Nonsense",
	"Madeon - The City",
	"Madeon - You're On ft. Kyan",
	"Maroon 5 - Don't Wanna Know",
	"Maroon 5 - Girls Like You",
	"Maroon 5 - If I Ain't Got You",
	"Maroon 5 - The Sun",
	"Maroon 5 - What Lovers Do",
	"Martin Solveig - The Night Out                             (Madeon Remix)",
	"MC Sniper - For You",
	"MC Sniper - Magic Castle",
	"MC Sniper - SKYFALL",
	"MC Sniper - Night Flight",
	"Muse - Stockholm Syndrome",
	"Muse - Uprising",
	"My Chemical Romance - Dead!",
	"My Chemical Romance - I'm Not Okay",
	"My Chemical Romance - Na Na Na",
	"Nate Rose & Sober - Yes Man",
	"ND Lee - Early bird",
	"Neon trees - Animal",
	"Neon trees - Everybody Talks",
	"NieN - Mellow D Fantasy",
	"Pendulum - The Island (Madeon Remix)",
	"PJ Morton - Heavy",
	"planetboom - Raise Me Up",
	"Queen - Bohemian Rhapsody",
	"Queen - Don't Stop Me Now",
	"Queen - I was born to love you",
	"Queen - Somebody To Love",
	"Queen - Under Pressure",
	"Sammy Adams - All Night Longer",
	"Sammy Adams - L.A. Story",
	"Sum 41 - Still Waiting",
	"Sum 41 - The Hell Song",
	"Trevor Wesley - Chivalry Is Dead",
	"Young Franco - Girls Don't Cry",
	"Gray - Just Do It (Feat. Loco)",
	"Geeks - Officially Missing You",
	"Geeks - How are you (Feat. Harim)",
	"GB9 - Star",
	"Naul - Baby Funk",
	"Naul - You From The Same Time",
	"Naul - Emptiness in Memory",
	"Naul - Feel Like",
	"Nuol - I Need A Beat (Per. Minos,                         Bizzy-B, MC Meta)",
	"Dynamic Duo - Without You",
	"Dynamic Duo - Be My Brownie",
	"Dynamic Duo - Go Back",
	"Drunken Tiger - formula",
	"Jung Jae yoon - Evolution",
	"Romantic Punch - Saturday night fever",
	"MAMAMOO - Girl Crush",
	"MAMAMOO - Love",
	"MAMAMOO - Wind Flower",
	"MAMAMOO - GoGoBeBe",
	"MAMAMOO - Woo Hoo",
	"MAMAMOO - Yes I Am",
	"MAMAMOO - I Love Too",
	"MAMAMOO - Star Wind Flower Sun",
	"MAMAMOO - Better than I thought",
	"MAMAMOO - Rainy Season",
	"MAMAMOO - Sky! Sky!",
	"Maktub - To You My Light",
	"Moonbyul - SELFISH",
	"Park Sang Min - crazy for you",
	"Park Wan Kyu - Lonely Night",
	"Park Wan Kyu - Love .... That Guy",
	"Park Eung Sik - Run",
	"Baechigi - Wonder Sometimes",
	"Baechigi - DA TA",
	"Baechigi - Do More (Feat. Babylon)",
	"Baechigi - DDURAEYO",
	"Baechigi - Turn A Deef Ear",
	"Baechigi - The Foul King",
	"Baechigi - Considered Climacteric",
	"Baechigi - This Room",
	"Brown Eyed Soul - Go",
	"Brown Eyes - with Coffee",
	"Beenzino - always awake",
	"Beenzino - How Do I Look",
	"Simon Dominic - Lonely Night",
	"Simon Dominic - Home Shit Home                          (feat. JINBO the superfreak)",
	"Seo In Guk - Time Machine",
	"Supreme Team - Step Up",
	"Supreme Team - You Can Stay",
	"IU - Someday",
	"IU - Secret Garden",
	"IU - Can't Love You Anymore                             (With. OHHYUK)",
	"AKMU - 200",
	"AKMU - Give Love",
	"Lee Seung Chul - Jasmine",
	"Tsra - Brave Heart",
	"Tsra - Fire",
	"Tsra - The biggest dreamer",
	"Tsra - Break up!",
	"Tsra - Target Akai Shougeki",
	"JANNABI - For Lovers Who Hesitat",
	"Jeon Young Ho - Butterfly",
	"ChangMo - Erase You",
	"Code Kunst - Perfume(feat. NUCKSAL)",
	"Yoo Jung Suk - Zli Poong Ga Do",
	"Two Months - Lady in the Rainy Night",
	"Pepper Tones - Ready, Get Set, Go!",
	"Pepper Tones - Super Fantastic",
	"Pepper Tones - Park Trip",
	"Pepper Tones - Good Morning Sandwichman",
	"Pepper Tones - Good Luck",
	"Primary & OHHYUK - Bawling",
	"Primary & OHHYUK - Gondry                                (feat. Lim Kim)",
	"Primary & OHHYUK - Rubber",
	"Primary - Love (Feat. Bumkey, Paloalto)",
	"Primary - Too Far (Feat  Beenzino)",
	"HYUK OH - Ohio",
	"HYUK OH - Gondry",
	"Acourve - What I Want To Say",
	"Bruno Mars - That's What I Like",
	"CHVRCHES - Gun",
	"CHVRCHES - The Mother We Share",
	"Dean - 21",
	"Dean - D (Half Moon)",
	"Dean - I'm Not Sorry",
	"Ed Sheeran - Cold Coffee",
	"Ed Sheeran - Shape of You",
	"Ed Sheeran - Sing",
	"Fall out boy - Sending Postcards from                     a Plane Crash",
	"Fall Out Boy - Stay Frosty Royal                          Milk Tea",
	"Fall Out Boy - Where Did The Party Go",
	"Fall Out Boy - Tiffany Blews",
	"Heize - And July",
	"Heize - Shut Up & Groove",
	"John Mayer - Still Feel Like Your Man",
	"Junoflo - Turn Up Tokyo",
	"Martin Garrix - Summer Days",
	"Mickey Cho - Thoughts",
	"One More Chance - Thinking of you",
	"Panic At The Disco - Into the Unknown",
	"Phum Viphurit - Lover Boy",
	"The Endorphins - All Night",
	"Tom Misch - Disco Yes",
	"Two Door Cinema Club - Undercover                         Martyn",
	"Two Two - One and a Half",
	"ZICO - Artist",
	"ZICO - Behind the scene",
	"ZICO - Human"
};

void setup(void) {
	randomSeed(analogRead(A12));
	Serial.begin(9600);
	tft.reset();
	uint16_t identifier = tft.readID();
	tft.begin(identifier);
	pinMode(45, INPUT_PULLUP);
	mySerial.begin(9600);
	init_();
}
void loop(void) {
	setVol();
	lockCheck();
	if (mp3On) {
		checkSongEnd();
	}
	if (!lock) {
		buttonCheck();
		stickCheck();
	}
}
void init_() {
	mp3.begin(mySerial);
	delay(1);
	mp3.volume(1);
	drawSquare_full(WHITE, BLUE, 3);
	drawIndex();
	updateEQ();
	updateRepeat();
	updateShuffle();
	for (int i = 0; i < SONG + 1; i++) {		//setting music List-- Linear
		musicList[i] = i + 1;
	}
	for (int i = 0; i < 10; i++) {		//print song name to index
		if (i == 0) { tft.setTextColor(RED); }
		else { tft.setTextColor(BLACK); }
		tft.setCursor(70, (40 * i) + 15);
		tft.print(song_name[i]);
	}
}
void setVol() {
	vol = map(analogRead(A10), 1023, 0, 0, 30);
	mp3.volume(vol);
	updateVol();
}
void setSong() {
	mp3.play(musicList[curMusicIndex]);
	paused = false;
	drawCurMusicState();
}
void buttonCheck() {
	if (buttonOn) {
		if (!digitalRead(49) && !buttonPushed) {
			buttonPushed = true;
			curMusicIndex = musicIndex;
			if (shuffleOn) { setShuffle(); }
			setSong();
			paused = false;
			printCurMusic();
			if (!mp3On) { mp3On = true; }
		}
		else if (!digitalRead(47) && !buttonPushed) {
			buttonPushed = true;
			if (option) {
				option = false;
				x_gap = 100; y_gap = 200;
				tft.fillCircle(30, 55, 3, WHITE);
				tft.fillCircle(30, 125, 3, WHITE);
				tft.fillCircle(30, 190, 3, WHITE);
			}
			else {
				option = true;
				optionIndex = 0;
				x_gap = 500;
				y_gap = 500;
				drawOptionIndex();
			}
		}
		else if (!digitalRead(45) && !buttonPushed) {
			buttonPushed = true;
			if (paused) { mp3.resume(); paused = false; Serial.println("resume"); mp3On = true; }
			else { mp3.pause(); paused = true; Serial.println("pause"); mp3On = false; }
			drawCurMusicState();
		}
		else if (digitalRead(49) && digitalRead(47) && digitalRead(45)) {
			buttonPushed = false;
			buttonOn = false;
			buttonOnChecker = false;
		}
	}
	else {
		if (!buttonOnChecker) {
			if (!digitalRead(49) || !digitalRead(47) || !digitalRead(45)) {
				buttonOnChecker = true;
				buttonTimer = millis();
			}
		}
		else {
			if (millis() - buttonTimer < 100) {
				if (digitalRead(49) && digitalRead(47) && digitalRead(45)) {
					buttonOn = false;
					buttonOnChecker = false;
				}
			}
			else { buttonOn = true; }
		}
	}
}
void stickCheck() {
	if (millis() - stickHtimer > y_gap) {		//y
		stickHtimer = millis();
		if (analogRead(A9) >= 700) {
			if (!option) {
				if (musicIndex != SONG) {
					if (musicIndex % 10 == 9) { musicIndex -= 9; }
					else { musicIndex++; }
				}
				else { musicIndex = (SONG / 10) * 10; }
				updateMusicIndex(1, 0);
			}
			else {
				if (optionIndex != 2) { optionIndex++; }
				else { optionIndex = 0; }
				drawOptionIndex();
			}
		}
		else if (analogRead(A9) <= 300) {
			if (!option) {
				if (musicIndex != (SONG / 10) * 10) {
					if (musicIndex % 10 == 0) { musicIndex += 9; }
					else { musicIndex--; }
				}
				else { musicIndex = SONG; }
				updateMusicIndex(0, 0);
			}
			else {
				if (optionIndex != 0) { optionIndex--; }
				else { optionIndex = 2; }
				drawOptionIndex();
			}
		}
	}
	if (millis() - stickVtimer > x_gap) {		//x
		stickVtimer = millis();
		if (analogRead(A8) >= 700) {
			if (!option) {
				if (musicIndex / 10 == SONG / 10) { musicIndex -= (SONG / 10) * 10; }
				else { musicIndex += 10; }
				updateMusicIndex(1, 1);
			}
			else {
				switch (optionIndex)
				{
				case 0:
					if (repeatOn) { repeatOn = false; }
					else { repeatOn = true; }
					updateRepeat();
					break;
				case 1:
					if (shuffleOn) { shuffleOn = false; }
					else { shuffleOn = true; }
					updateShuffle();
					break;
				default:
					if (EQ_ != 5) { EQ_++; }
					else { EQ_ = 0; }
					updateEQ();
					break;
				}
			}
		}
		else if (analogRead(A8) <= 300) {
			if (!option) {
				if (musicIndex / 10 == 0) { if (musicIndex == 9) { musicIndex = SONG; } else { musicIndex += (SONG / 10) * 10; } }
				else { musicIndex -= 10; }
				updateMusicIndex(1, 1);
			}
			else {
				switch (optionIndex)
				{
				case 0:
					if (repeatOn) { repeatOn = false; }
					else { repeatOn = true; }
					updateRepeat();
					break;
				case 1:
					if (shuffleOn) { shuffleOn = false; }
					else { shuffleOn = true; }
					updateShuffle();
					break;
				default:
					if (EQ_ != 0) { EQ_--; }
					else { EQ_ = 5; }
					updateEQ();
					break;
				}
			}
		}
	}
}
void checkSongEnd() {
	if (digitalRead(43) == HIGH) {
		if (!repeatOn) {
			if (curMusicIndex == SONG) { curMusicIndex = 0; }
			else { curMusicIndex++; }
		}
		setSong();
		Serial.println("next song");
		printCurMusic();
	}
}
void drawSquare_full(int color, int color2, int thickness_) {
	thickness = thickness_;
	tft.fillScreen(color);
	tft.fillRect(3, 3, WIDTH - 3, HEIGHT - 3, color2);
	tft.fillRect(thickness + 3, thickness + 3, WIDTH - (thickness + 6), HEIGHT - (thickness + 6), color);
}
void drawIndex() {
	tft.drawLine(60, thickness + 3, 60, HEIGHT - (thickness + 1), BLACK);
	tft.drawLine(thickness + 3, 400, WIDTH - (thickness + 4), 400, BLACK);
	for (int i = 0; i < 10; i++) {
		tft.drawLine(60, 40 * i + 40, WIDTH - thickness, 40 * i + 40, BLACK);
	}
	for (int i = 0; i < 3; i++) {
		tft.drawLine(thickness + 3, (66 * i) + 68, 60, (66 * i) + 68, BLACK);
	}
}
void updateMusicIndex(bool up, bool next) {
	tft.setTextSize(1);
	for (int i = 0; i < 10; i++) {
		if (next) { tft.fillRect(65, (40 * i) + 10, 248, 30, WHITE); }
		if (i == musicIndex % 10) { tft.setTextColor(RED); }
		else { tft.setTextColor(BLACK); }
		tft.setCursor(70, (40 * i) + 15);
		if (i != 9 || musicIndex / 10 != SONG / 10) { tft.print(song_name[(musicIndex / 10) * 10 + i]); }
		/*												for better performance  recommand to erase
		tft.setTextColor(RED);
		tft.setCursor(65, (40 * musicIndex % 10 + 40) + 15);
		tft.print(song_name[(musicIndex - 1)]);
		tft.setTextColor(BLACK);
		if(!next){
		if (up) {tft.setCursor(65, (40 * (musicIndex - 1) % 10 + 40) + 15); tft.print(song_name[((musicIndex-1) - 1)]);}
		else {tft.setCursor(65, (40 * (musicIndex + 1) % 10 + 40) + 15); tft.print(song_name[((musicIndex+1) - 1)]);}
		}
		*/
	}
}
void updateShuffle() {
	tft.setTextSize(1);
	tft.setCursor(14, 100);
	if (shuffleOn) { (tft.setTextColor(RED)); setShuffle(); }
	else { tft.setTextColor(BLACK); for (int i = 0; i < SONG + 1; i++) { musicList[i] = i + 1; } }
	tft.print("shuffle");
}
void updateRepeat() {
	tft.setTextSize(1);
	tft.setCursor(14, 34);
	if (repeatOn)(tft.setTextColor(RED));
	else { tft.setTextColor(BLACK); }
	tft.print("repeat");
}
void updateEQ() {
	tft.setTextSize(1);
	tft.setTextColor(BLACK);
	tft.fillRect(8, 164, 50, 10, WHITE);
	switch (EQ_)
	{
	case 0:tft.setCursor(16, 165); tft.print("normal"); break;
	case 1:tft.setCursor(23, 165); tft.print("pop"); break;
	case 2:tft.setCursor(18, 165); tft.print("rock"); break;
	case 3:tft.setCursor(18, 165); tft.print("jazz"); break;
	case 4:tft.setCursor(12, 165); tft.print("classic"); break;
	case 5:tft.setCursor(18, 165); tft.print("base"); break;
	}
	mp3.EQSelect(EQ_);
}
void updateVol() {
	if (curVol != vol) {
		if (curVol >= vol) {
			tft.fillRect(10, 398 - ((curVol) * 6), 30, 4, WHITE);
			curVol--;
		}
		else if (curVol <= vol) {
			tft.fillRect(10, 398 - ((curVol + 1) * 6), 30, 4, BLACK);
			curVol++;
		}
	}
}
void setShuffle() {
	for (int i = 0; i < SONG + 1; i++) {			//shuffle 2
		byte x = random(SONG) + 1;
		byte y = musicList[i];
		if ((i + x) < SONG) {
			musicList[i] = musicList[i + x];
			musicList[i + x] = y;
		}
		else {
			musicList[i] = musicList[(i + x) - SONG];
			musicList[(i + x) - SONG] = y;
		}
	}
	for (int i = 0; i < SONG + 1; i++) {			//line up the index
		if (musicList[i] == curMusicIndex + 1) {
			byte x = musicList[0];
			musicList[0] = musicList[i];
			musicList[i] = x;
			curMusicIndex = 0;
			break;
		}
	}
}
void printCurMusic() {		// need to modulate point
	tft.setTextSize(1);
	tft.setTextColor(BLACK);
	tft.fillRect(65, 405, 245, 50, WHITE);
	tft.setCursor(70, 425);
	tft.setTextColor(MAGENTA);
	tft.print(song_name[musicList[curMusicIndex] - 1]);
	tft.setTextColor(BLACK);
}
void drawCurMusicState() {
	tft.fillRect(15, 410, 40, 50, WHITE);
	if (!paused) {
		tft.fillRect(20, 420, 10, 30, BLACK);
		tft.fillRect(40, 420, 10, 30, BLACK);
	}
	else { tft.fillTriangle(20, 420, 20, 450, 50, 435, BLACK); }
}
void drawOptionIndex() {
	tft.fillCircle(30, 55, 3, WHITE);
	tft.fillCircle(30, 125, 3, WHITE);
	tft.fillCircle(30, 190, 3, WHITE);
	switch (optionIndex)
	{
	case 0:
		tft.fillCircle(30, 55, 3, RED);
		break;
	case 1:
		tft.fillCircle(30, 125, 3, RED);
		break;
	default:
		tft.fillCircle(30, 190, 3, RED);
		break;
	}
}
void lockCheck() {
	if (digitalRead(41)) { lock = true; }
	else { lock = false; }
}