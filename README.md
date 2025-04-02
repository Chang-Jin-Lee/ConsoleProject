# AsciiNikke [ConsoleProject]

- 개발 기간 : 7일 (2025. 03. 25 ~ 03. 31)
- 개발 인원 : 1명
- 개발 목적 : 콘솔 프로그래밍 학습
- 스택 : Windows Console API, ffmpeg, FMOD, AsciiGen2

- 동영상링크
    https://youtu.be/KQgX1lqgpaI

- 참고
    - 영상을 아스키 아트로 실행하는 원리
        1. ffmpeg로 영상을 60프레임 png 파일로 변환
        2. 변환된 png 파일들을 AsciiGen2 batch를 사용해 txt 파일로 전부 전환
        3. C 스타일로 동적할당받아 파일을 읽으면서 아스키 아트의 string을 저장
        4. WriteConsoleOutputCharacterA API로 값 출력

    - 콘솔 화면 크기를 맞추는 방법
        1. 1920 * 1080 사이즈에 Ratio를 곱하여 적용
        2. SetConsoleWindowInfo 함수로 핸들, 버퍼의 사이즈를 지정하고
        3. SetCurrentConsoleFontEx 로 폰트 사이즈를 변환하면 된다
        4. Ratio, FontSize를 변경하여 인게임 내에서 해상도를 변경 가능