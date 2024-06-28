#ifndef CAR_EMOTIONS_H
#define CAR_EMOTIONS_H

#include <Freenove_VK16K33_Lib.h>

#define EMOTION_ADDRESS 0x71
#define EMOTION_SDA 13
#define EMOTION_SCL 14

extern byte eyeRotate1[][8];
extern byte eyeRotate2[][8];
extern byte eyeBlink[][8];
extern byte eyeSmile[][8];
extern byte eyeCry1[][8];
extern byte eyeCry2[][8];
extern byte cat_left[][8];
extern byte cat_right[][8];
extern byte static_emotion_left[][8];
extern byte static_emotion_right[][8];
extern byte clearEmotion[][8];

extern Freenove_VK16K33 matrix;
extern int emotion_time_next;
extern int emotion_time_now;
extern int emotion_count;
extern int emotion_task_mode;
extern int emotion_mode;

void eyesRotate(int delay_ms);
void eyesBlink(int delay_ms);
void eyesSmile(int delay_ms);
void eyesCry(int delay_ms);
void cat(int delay_ms);
void staticEmtions(int emotion);
void clearEmtions(void);
void Emotion_Setup();
void Emotion_Show(int mode);
void Emotion_SetMode(int mode);

#endif // CAR_EMOTIONS_H
