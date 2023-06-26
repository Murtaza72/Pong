#include <raylib.h>
#include <string>

struct Ball
{
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw()
	{
		DrawCircle((int) x, (int) y, radius, WHITE);
	}
};

struct Paddle
{
	float x, y;
	float width, height;
	float speed;

	Rectangle GetRec()
	{
		return Rectangle{ x - width / 2, y - height / 2,  width,  height };
	}

	void Draw()
	{
		DrawRectangleRec(GetRec(), BLUE);
	}
};

int main()
{
	InitWindow(800, 600, "Pong");
	SetWindowState(FLAG_VSYNC_HINT);

	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.speedX = 300.0;
	ball.speedY = 300.0;
	ball.radius = 5.0;

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2.0f;
	leftPaddle.width = 10.0;
	leftPaddle.height = 100.0;
	leftPaddle.speed = 600;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50.0f;
	rightPaddle.y = GetScreenHeight() / 2.0f;
	rightPaddle.width = 10.0;
	rightPaddle.height = 100.0;
	rightPaddle.speed = 600;

	const char* winnerText = nullptr;
	int leftScore = 0;
	int rightScore = 0;

	while (!WindowShouldClose()) {

		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		// Stop the ball from going off the screen
		if (ball.y < 0) {
			ball.y = 0.0f;
			ball.speedY *= -1;
		}

		if (ball.y > GetScreenHeight()) {
			ball.y = GetScreenHeight() * 1.0f;
			ball.speedY *= -1;
		}

		if (ball.x < 0) {
			winnerText = "Right Player Wins!";
		}

		if (ball.x > GetScreenWidth()) {
			winnerText = "Left Player Wins!";
		}

		if (IsKeyDown(KEY_W)) {
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_S)) {
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_A)) {
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_D)) {
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}

		// Check if paddles are in screen bounds
		if (leftPaddle.y - leftPaddle.height / 2 < 0) {
			leftPaddle.y = leftPaddle.height / 2;
		}

		if (leftPaddle.y + leftPaddle.height / 2 > GetScreenHeight()) {
			leftPaddle.y = GetScreenHeight() - leftPaddle.height / 2;
		}

		if (rightPaddle.y - rightPaddle.height / 2 < 0) {
			rightPaddle.y = rightPaddle.height / 2;
		}

		if (rightPaddle.y + rightPaddle.height / 2 > GetScreenHeight()) {
			rightPaddle.y = GetScreenHeight() - rightPaddle.height / 2;
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, leftPaddle.GetRec())) {
			if (ball.speedX < 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
				leftScore++;
			}

		}

		if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, rightPaddle.GetRec())) {
			if (ball.speedX > 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
				rightScore++;
			}
		}

		if (winnerText && IsKeyDown(KEY_SPACE)) {
			ball.x = GetScreenWidth() / 2.0f;
			ball.y = GetScreenHeight() / 2.0f;
			ball.speedX = 300.0;
			ball.speedY = 300.0;
			winnerText = nullptr;
			leftScore = 0;
			rightScore = 0;
		}

		BeginDrawing();

		{
			ClearBackground(BLACK);

			ball.Draw();
			leftPaddle.Draw();
			rightPaddle.Draw();

			char l[10];
			char r[10];
			snprintf(l, sizeof l, "%d", leftScore);
			snprintf(r, sizeof r, "%d", rightScore);

			DrawText(l, 200, 10, 20, PINK);
			DrawText(r, GetScreenWidth() - 200, 10, 20, PINK);

			if (winnerText) {
				int textWidth = MeasureText(winnerText, 50);
				DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2, 50, GREEN);
			}

			DrawFPS(10, 10);
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}