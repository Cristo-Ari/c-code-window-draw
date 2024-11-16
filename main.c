#include <windows.h>
#include <math.h>  // Для математических функций (sin, cos)

#define TIMER_ID 1  // Идентификатор таймера для обновления экрана

// Параметры вращающихся кругов
float angle1 = 0.0f;  // Начальный угол первого круга
float angle2 = 3.14159f;  // Начальный угол второго круга (стартует с другой позиции)

// Скорости вращения
float speed1 = 0.05f;  // Скорость первого круга
float speed2 = 0.0167f; // Скорость второго круга (в 3 раза медленнее)

// Радиус вращения
int radius = 100;

// Функция для проверки столкновения кругов
int checkCollision(int x1, int y1, int x2, int y2, int radius) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    int distanceSquared = dx * dx + dy * dy;
    int collisionDistanceSquared = (2 * radius) * (2 * radius); // Радиус двух кругов

    return distanceSquared <= collisionDistanceSquared;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static int direction1 = 1;  // Направление первого круга (1 - по часовой стрелке, -1 - против часовой)
    static int direction2 = 1;  // Направление второго круга

    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Получаем размеры окна
            RECT rect;
            GetClientRect(hwnd, &rect);
            int centerX = (rect.left + rect.right) / 2;
            int centerY = (rect.top + rect.bottom) / 2;

            // Вычисляем новые координаты для первого круга
            int x1 = (int)(centerX + radius * cos(angle1));
            int y1 = (int)(centerY + radius * sin(angle1));

            // Рисуем первый круг
            Ellipse(hdc, x1 - 25, y1 - 25, x1 + 25, y1 + 25);

            // Вычисляем новые координаты для второго круга
            int x2 = (int)(centerX + radius * cos(angle2));
            int y2 = (int)(centerY + radius * sin(angle2));

            // Рисуем второй круг
            SelectObject(hdc, GetStockObject(DC_PEN));
            SetDCPenColor(hdc, RGB(255, 0, 0)); // Красный цвет для второго круга
            Ellipse(hdc, x2 - 25, y2 - 25, x2 + 25, y2 + 25);

            EndPaint(hwnd, &ps);

            // Проверка на столкновение
            if (checkCollision(x1, y1, x2, y2, 25)) {
                // Если столкновение, меняем направления движения обоих кругов
                direction1 *= -1;
                direction2 *= -1;
            }
            break;
        }
        case WM_TIMER: {
            // Обновляем угол первого круга
            angle1 += speed1 * direction1;
            if (angle1 >= 2 * 3.14159f) angle1 = 0.0f;  // Ограничиваем угол до 360 градусов

            // Обновляем угол второго круга
            angle2 += speed2 * direction2;
            if (angle2 >= 2 * 3.14159f) angle2 = 0.0f;

            // Запрашиваем перерисовку окна
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int main() {
    char lpszClassName[] = "windo123w";
    WNDCLASSA wc = {
        .lpfnWndProc = WndProc,
        .hInstance = GetModuleHandle(NULL),
        .lpszClassName = lpszClassName
    };
    RegisterClassA(&wc);

    // Создаем окно
    HWND hwnd = CreateWindowA(
        lpszClassName,
        "Window with Rotating Circles",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        640,
        480,
        NULL,
        NULL,
        NULL,
        NULL
    );
    ShowWindow(hwnd, SW_SHOWNORMAL);

    // Устанавливаем таймер для обновления экрана (каждые 16 мс)
    SetTimer(hwnd, TIMER_ID, 16, NULL);  // 1000/60 ≈ 16 ms для 60 FPS

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Убираем таймер перед завершением
    KillTimer(hwnd, TIMER_ID);
    return 0;
}
