# Tower Defense Game

使用 C++ 和 raylib 製作的 2D 塔防遊戲。

## 遊戲說明

- 左側有一座城堡，需要保護它
- 中間是玩家角色，可以移動和射擊
- 右邊會生成敵人，往城堡方向前進
- 在敵人到達城堡之前擊敗他們！

## 操作方式

- `A` / `D` — 左右移動
- `W` — 跳躍
- 滑鼠左鍵 — 朝游標方向射擊（有冷卻時間）

## 編譯與執行
已經有 build 好的檔案，在 \build\TowerDefenseGame.exe
```console
cmake -B build
cmake --build build
./build/TowerDefenseGame
```