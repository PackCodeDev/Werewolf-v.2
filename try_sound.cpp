#include <windows.h>

int main() {
    if (!PlaySound(TEXT("sound.wav"), NULL, SND_FILENAME | SND_ASYNC)) {
        MessageBox(NULL, TEXT("ไม่สามารถเล่นเสียงได้"), TEXT("ข้อผิดพลาด"), MB_OK);
    }
    return 0;
}

