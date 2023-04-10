# 아두이노로 구현한 mp3Player

#### 사용 기술: visualMicro

#### 사용 부품
 - 3.5인치 TFT모니터
 - 가변저항
 - 아날로그 스틱
 - DFPlayer
 - 스피커
 - 오디오잭

<img width="30%" src="https://user-images.githubusercontent.com/33209821/230914477-c948d90d-5d57-43e7-9fcf-1e804798eec5.jpg"/>

#### 설명
 - 모니터의 가로, 세로, 음악 수는 전처리한다.
 ```C++
 #define WIDTH 317
 #define HEIGHT 477
 #define SONG 198// 원래 169개 였는데 후에 추가함 따라서 musicList 배열 길이 수정 필요
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
 
 - 아날로그 입력은 페이지 이동을 담당한다.
   - 연속 입력을 방지하기위해 시간 간격을 두고 입력을 받는다.
   - 글리치를 무시하기 위해 특정 범위 내 값만 받는다.
 - 버튼 입력은 연속 입력을 방지하기 위해 최초 키다운만 감지한다.
 - 현재 재생중인 음악은 빨간색 글자로 출력한다.
 - 볼륨 조절은 가변저항의 출력 전압을 0~30으로 맵핑하여 사용한다.
 ```C#
 vol = map(analogRead(A10), 1023, 0, 0, 30);
 ```
 - 볼륨 상태창은 다음과 같이 구현했다.
 ```C#
 if (curVol >= vol) {// 볼륨을 낮췄을 시
	tft.fillRect(10, 398 - ((curVol) * 6), 30, 4, WHITE);// 가장 위 막대를 지움
	curVol--;
	}
else if (curVol <= vol) {
	tft.fillRect(10, 398 - ((curVol + 1) * 6), 30, 4, BLACK);// 가장 위 막대 추가
	curVol++;
	}
 ```
 - 셔플 기능은 각 인덱스 마다 랜덤한 위치에 있는 곡과 위치를 바꾸어 구현했다.(setShuffle())
   - 셔플 시 현재 재생되고있는 음악과 배열의 첫번째 음악을 바꾼다.
```C++
byte x = random(SONG) + 1;// 노래 갯수만큼 랜덤한 값을 받음
byte y = musicList[i];
if ((i + x) < SONG) {// i+x가 인덱스 범위 내
	musicList[i] = musicList[i + x];// i번째 노래와 i+x번째 노래를 바꿈
	musicList[i + x] = y;
}
else {
	musicList[i] = musicList[(i + x) - SONG];
	musicList[(i + x) - SONG] = y;
	}

```

