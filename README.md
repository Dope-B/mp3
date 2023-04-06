# 아두이노로 구현한 mp3Player

#### 사용 기술: visualMicro

#### 사용 부품
 - 3.5인치 TFT모니터
 - 가변저항
 - 아날로그 스틱
 - DFPlayer
 - 스피커
 - 오디오잭

#### 설명
 - 모니터의 가로, 세로, 음악 수, 는 전처리한다.
 ```C++
 #define WIDTH 317
 #define HEIGHT 477
 #define SONG 198
 ```
 - 주요 기능은 다음과 같다.
  - EQ 설정
  - 셔플 재생
  - 반복 재생
  - 볼륨 조절
  - 일시정지, 이어듣기
  - 오디오잭 지원
  - 버튼 잠금
  - 스피커 잠금
- 노래는 sd카드에서 파일을 읽은 뒤 play함수로 실행한다.
```C++
void setSong() {
	mp3.play(musicList[curMusicIndex]);// 해당 노래 제목 배열에 있는 문자열과 같은 이름을 가진 파일 재생  
	paused = false;// 멈춤 상태 해제
	drawCurMusicState();// 현재 상태에 따라 화면 그리기
}

void drawCurMusicState() {// 재생 상태 인지 멈춤 상태인지 출력
	tft.fillRect(15, 410, 40, 50, WHITE);// 상태 지움 
	if (!paused) {// 재생 상태일 시
		tft.fillRect(20, 420, 10, 30, BLACK);// 재생 상태 모양(ㅣㅣ) 출력
		tft.fillRect(40, 420, 10, 30, BLACK);
	}
	else { tft.fillTriangle(20, 420, 20, 450, 50, 435, BLACK); }// 멈춤 상태일 시 삼각형 출력
}
```

- 기기 실행 시 최초로 다음과 같은 함수가 실행된다.
 - 셔플 기능을 위한 랜덤 시드 받기
 - mp3모듈 초기화
 - 볼륨 1로 설정
 - 화면 초기화
 - UI 출력
 - 옵션 상태 초기화 및 해당 UI 출력
 - 재생 리스트 할당
 - UI에 노래 제목 출력
 
 - 루프중 실행되는 함수는 다음과 같다.
  - 볼륨 체크
  - 버튼 잠금 체크
  - 음악 종료 감지
  - 버튼 및 아날로그 스틱 입력 체크
 
 
 - 현재 재생중인 음악은 빨간색 글자로 출력한다.
 - UI는 tft 내장 함수를 사용한다.(drawIndex())
 - 볼륨 조절은 가변저항의 출력 전압을 0~30으로 맵핑하여 사용한다.
 ```C#
 vol = map(analogRead(A10), 1023, 0, 0, 30);
 ```
 - 
 - 
 - 

