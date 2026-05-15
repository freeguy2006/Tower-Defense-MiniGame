# Tower Defense Game

使用 C++ 和 raylib 製作的 2D 塔防遊戲。

## 遊戲說明

- 左側有一座城堡，需要保護它
- 中間是玩家角色，可以移動和射擊
- 右邊會生成敵人，往城堡方向前進
- 在敵人到達城堡之前擊敗他們！

## 功能

- 玩家：移動、跳躍、朝滑鼠方向射擊（有冷卻時間）
- 敵人：自動從右側生成，往城堡方向前進
- 碰撞偵測（子彈 vs 敵人、敵人 vs 城堡、敵人 vs 玩家）
- 血條 HUD（城堡 HP、玩家 HP、擊殺數）
- 遊戲狀態畫面（開始、遊玩中、暫停、結束） (還沒做暫停)

## 操作方式

- `A` / `D` — 左右移動
- `W` — 跳躍
- 滑鼠左鍵 — 朝游標方向射擊（有冷卻時間）

## UML Class Diagram

- `UML/assignment5_UML.pdf` — Assignment 5 繳交時的架構
- `UML/TowerDefense_UML.pdf` — 目前整個遊戲的完整架構

## 編譯與執行
已經有 build 好的檔案，在 \build\TowerDefenseGame.exe
```console
cmake -B build
cmake --build build
./build/TowerDefenseGame
```