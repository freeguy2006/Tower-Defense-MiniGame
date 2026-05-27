# Tower Defense Game

使用 C++ 和 raylib 製作的 2D 塔防射擊遊戲。

## 遊戲說明

- 保護左邊的城堡不被怪物摧毀
- 玩家可以移動、跳躍、朝游標射擊
- 波次制：撐過所有波次即獲勝
- 擊殺敵人掉落金幣，升級能力和購買武器/藥水

## 操作方式

| 按鍵 | 功能 |
|------|------|
| A / D | 左右移動 |
| W | 跳躍 |
| 滑鼠左鍵 | 射擊 |
| ESC | 暫停 / 升級商店 |
| 1~7 | 購買升級 |
| Enter | 開始 / 繼續 (教學頁面狂按 50 下解鎖彩蛋) |
| G | 教學 |
| H | Debug 碰撞箱 (顯示 AABB 綠色框線) |
| F | 回饋表單 |

## 敵人（9 種）

| 名稱 | 代號 | 血量 | 速度 | 特色 |
|------|------|------|------|------|
| 綠色史萊姆 | **LGN** | 70 | 快 | 陸地移動 + 小跳 |
| 黑色史萊姆 | **LBK** | 500 | 慢 | 陸地移動 + 高血量坦克 |
| 紅色史萊姆 | **LRD** | 40 | 極快 | 陸地移動 + 小跳 |
| 紫色史萊姆 | **LPE** | 200 | 中 | 陸地移動 + 小跳與大跳交替 |
| 藍色史萊姆 | **LBE** | 150 | 中 | 陸地移動 + 增傷/加速/減傷光環 |
| 天使 | **SAL** | 170 | 中 | 天空飛行 + 範圍治療 |
| 小鳥 | **SBD** | 40 | 快 | 天空飛行 + 快速移動 |
| 飛龍 | **SDN** | 1200 | 慢 | Boss 級天空飛行 |
| 旋風怪 | **SWD** | 500 | 靜止 | 下墜落定後提供 X 軸大範圍牽引風暴 |

## 武器（11 種）

| 武器 | 費用 | 效果 |
|------|------|------|
| 泥巴球 | 免費 | 基礎武器，微緩速 |
| 弓箭 | 150 | 高射速 |
| 石箭 | 150 | 高傷害 |
| 冰緩箭 | 100 | 60% 緩速 |
| 毒箭 | 100 | 疊加毒傷 |
| 穿透箭 | 150 | 穿透所有敵人 |
| 鐵球 | 100 | 3% 暴擊吃 90% 血量 |
| 火球 | 150 | 範圍爆炸 |
| 火箭 | 100 | 10% 暴擊 ×5 |
| 冰凍箭 | 100 | 冰凍 1 秒 |
| 飛彈 | 150 | 大範圍爆炸，暴擊 ×5，冷卻較長 |

## 藥水（7 種）

| 藥水 | 費用 | 效果 |
|------|------|------|
| 治療玩家 | 20 | 回滿玩家血量 |
| 治療城堡 | 20 | 回滿城堡血量 |
| 攻擊強化 | 30 | 傷害倍率提升（持續數波） |
| 攻速強化 | 30 | 攻擊速度提升（持續數波） |
| 護盾 | 40 | 減少受到傷害（持續數波） |
| 移速強化 | 20 | 移動速度提升（持續數波） |
| 再生 | 30 | 持續回血（持續數波） |

## 升級商店（ESC 開啟）

| 按鍵 | 項目 | 基礎費用 |
|------|------|---------|
| 1 | 攻擊力 +1 | 20 |
| 2 | 玩家血量 +30 | 20 |
| 3 | 城堡血量 +100 | 20 |
| 4 | 金幣上限 +50 | 50 |
| 5 | 攻擊速度 | 80 |
| 6 | 多重射擊 +1 | 150 |
| 7 | 移動速度 | 50 |

每項可多次升級，費用隨等級遞增。

## 特殊機制與隱藏彩蛋

*   **新手教學神秘寶箱（Easter Egg）**：
    *   在**新手教學（TUTORIAL）**頁面連續狂按 `Enter` 鍵超過 **50 次**，重開遊戲後，天空中會以真實物理重力砸下一個超大的**神秘禮物盒**！
    *   玩家走過去撿起後，直接解鎖極致 OP 的逆天獎勵：金幣拉滿至 **`9999`**、玩家血量及城堡血量上限永久暴增並補滿至 **`9999`**、玩家基礎攻擊力永久暴增 **`+200`**！
*   **動態波次金幣加成**：怪獸身上掉落的金幣與該波次難度倍率 `hp_multiplier` 掛鉤，隨著遊戲難度提升，後期怪物掉落的金幣金額將動態成倍遞增！
*   **關卡勝利王冠**：當最後一波怪物全數剿滅後，天空中會緩緩落下一頂**金色勝利王冠**。玩家必須親自走過去碰撞王冠才能觸發最後的勝利畫面，帶來極致的通關儀式感！

## 波次系統與配置

關卡資訊從 `resources/levels.txt` 讀取，並可透過 `resources/level_editor.html` 進行高質感的視覺化滑鼠拖曳編輯。

語法：`cooldown 怪物代號*數量`（例如：`0.6 SBD*10` 代表間隔 0.6 秒生成 10 隻小鳥）

**九大系統代號對照表：**
*   **陸地怪系列 (Land)**：`LGN`(綠) 、 `LBK`(黑) 、 `LRD`(紅) 、 `LBE`(藍) 、 `LPE`(紫)
*   **天空怪系列 (Sky)**：`SAL`(天使) 、 `SBD`(小鳥) 、 `SDN`(飛龍) 、 `SWD`(風)

---

## 🏛️ 專案實體檔案結構 (Clean Architecture)

```
main.cpp             → 遊戲進入點：生命週期管理 g.init() -> g.run() -> g.close()
Game.h/.cpp          → 遊戲中央核心：主循環、碰撞判定、渲染緩衝、UI、音樂與狀態機
GameFactory.h        → 物件創始工廠：透過靜態成員與多元多型統一實例化與 constexpr 配置
Player.h/.cpp        → 玩家實體：實作輸入、物理位移、重力跳躍與武器發射
BehaviorAbstract.h   → 核心解耦行為抽象介面：用於切斷 Behavior <-> Enemy 循環包含依賴鏈
Behaviors.h          → 行為實現類別庫：JumpBehavior, BuffBehavior, HealBehavior, FallAndFloatBehavior 策略模式實現
Enemy.h              → 敵人純抽象基底類別：封裝 Buff/Debuff（冰/緩/毒）狀態，宣告純虛擬更新行為
EnemyLand.h          → 陸地怪具體衍生類別：實作地面水平移動與策略行為觸發
EnemySky.h           → 天空怪具體衍生類別：實作天空正弦波 2D 位移與策略行為觸發
Projectile.h         → 投射物：高度擴充的子彈類別（可選穿透、範圍爆炸、緩速、冰凍、疊毒、HP%暴擊）
DamageText.h         → 傷害漂浮文字：動態漸變傷害數值視覺表現
Wave.h               → 關卡波次載體
Coin.h               → 金幣實體
Goblin.h             → 商店哥布林 NPC
SpecialThings.h      → 物理互動地圖物件（金王冠、彩蛋禮物盒，繼承自 GameObject）
Castle.h / Building.h / Character.h / GameObject.h / Health.h  → 基礎繼承物件層級與防護核心
```

---

## 💎 OOP 與軟體工程高級設計觀念 (OOP & Design Patterns)

為了體現極致的軟體品質，本專案在物件導向程式設計（OOP）與設計模式（Design Patterns）上進行了精雕細琢：

*   **純抽象基底類別與動態繫結 (Dynamic Binding)**：
    `class enemy` 被重構為純抽象基底類別（Abstract Base Class），將更新邏輯定義為純虛擬函式 `virtual void update(float dt) = 0;`。並由 `land_enemy` 與 `sky_enemy` 進行平行繼承覆寫。主遊戲只透過 `enemy*` 基底指標進行控制，實踐里氏替換原則（Liskov Substitution Principle）。
*   **策略模式 (Strategy Pattern)**：
    敵人的特殊行為（小跳、大跳、範圍治療、屬性光環、落地風暴）全部抽離為 `enemy_behavior` 策略介面。這使得任意怪物都可以藉由 `add_behavior` 在執行期（Runtime）動態組合、掛載不同的行為策略，具備強大的擴充性。
*   **記憶體管理安全性 (Memory Safety)**：
    基底類別寫死 `virtual ~enemy()` 虛擬解構子，確保多型刪除 `delete enemy_ptr` 時能 100% 誘發衍生類別與掛載 Behaviors 的解構釋放，達成 **0 記憶體洩漏**。
*   **多層包含無環依賴 (DAG Include Structure)**：
    本專案透過 `BehaviorAbstract.h` 前置宣告打破了傳統 C++ 設計中「行為需要敵人、敵人需要行為」的循環包含（Circular Include）致命死結，整理出乾淨的包含單向鏈。
*   **組合大於繼承 (Composition over Inheritance)**：
    `Character` 組合了 `Health`（血量元件）；`Enemy` 組合了 `std::vector<enemy_behavior*>`，將複雜功能交由專職物件處理，避免類別膨脹。
*   **狀態模式 (State Machine)**：
    利用 `game_statement` 有限狀態機（START $\rightarrow$ TUTORIAL $\rightarrow$ PLAYING $\rightarrow$ PAUSE $\rightarrow$ WIN / LOSE），完美管理遊戲的生命週期與轉移邏輯。

---

## 資源結構

```
resources/
├── background/       — 背景圖片
├── object/           — 所有遊戲物件圖片（玩家、敵人、武器、藥水、城堡、金幣、哥布林）
├── monster/          — 怪物動畫幀（angel / bird / dragon）
├── music/
│   ├── BGM/          — 7 首 GameBoy 風格背景音樂
│   └── SoundEffect/
│       ├── get-coin/ — 撿金幣音效
│       └── wave-horn/— 波次開始號角
├── other/            — 備用素材
├── levels.txt        — 關卡設定
└── level_editor.html — HTML 視覺化關卡編輯器
```

## 編譯與執行

```console
cmake -B build
cmake --build build
./build/TowerDefenseGame
```

## 聲明

- 圖片素材：PixelLab AI / Lovart AI 生成
- 背景音樂：[OtoLogic GB-Shooter Series](https://otologic.jp/free/bgm/game-shooter-gb01.html)（免費授權）
- 音效：Pixabay（免費授權）
