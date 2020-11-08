MiHaJong 役の追加について
=========================


　この文書は、MiHaJong のソースコードを修正して役を追加する方法を解説したものです。

**注意：この文書は開発者向けです。エンドユーザーにとって意味のある情報は書かれていません。**





面子役（4面子1雀頭になっている役）の追加方法
============================================


　面子役を追加するには、ソースコードに追記する必要があります。次のファイルを変更します。

- mjcore/data/confitem.csv
- 次のファイルのうちの一つ
    - mjcore/yaku/catalog/triplet[1-3].cpp (刻子系の役はこのファイルに書かれています)
    - mjcore/yaku/catalog/quad.cpp (槓子系の役はこのファイルに書かれています)
    - mjcore/yaku/catalog/sequence.cpp (順子系の役はこのファイルに書かれています)
    - mjcore/yaku/catalog/suit.cpp (染め系の役はこのファイルに書かれています)
    - mjcore/yaku/catalog/misc.cpp (順子と刻子にまたがる役など、上記のどれにも当てはまらない手役はこのファイルに書かれています)

　ここでは、「ほげほげ」という順子役を追加するとします。


mjcore/data/confitem.csv
------------------------

　ここでは、CSVファイルにレコードを追加します。カンマ区切りで次のようになっています（レコード番号は0から始まるものとする）。

|レコード|意味                                       |備考                                 |
|-------:|:------------------------------------------|:------------------------------------|
|       0|項目番号                                   |10進非負整数                         |
|       1|適用するクライアントのマスク               |10進非負整数                         |
|       2|N/Aにするクライアントのマスク              |10進非負整数                         |
|       3|マスク条件を表すLuaのコード                |コード片は必ず二重引用符で囲むこと   |
|       4|ページ見出し(項目番号 % 40 == 0 の場合のみ)|項目番号 % 40 != 0 の場合は無視される|
|    5～7|予約                                       |何も入力しないこと                   |
|       8|識別用記号（英字）                         |項目番号ごとに一意であること         |
|       9|項目見出し                                 |                                     |
|      10|項目の説明                                 |                                     |
|    11～|設定項目（英字の識別記号）                 |mjcore/data/confitem.iniを参照のこと |

### クライアントのマスク ###

　クライアントのマスクは、次のビット和の10進表記です。

|ビット|十進|十六進|意味      |
|-----:|---:|-----:|:---------|
|     0|   1|  0x01|四人打ち  |
|     1|   2|  0x02|三人打ち  |
|     2|   4|  0x04|四人三麻  |
|     3|   8|  0x08|数牌三麻  |
|     4|  16|  0x10|瀬戸内三麻|

　レコード番号1には、適用するクライアントの種類を指定します。例えば、15を指定すると4種全てのクライアントで使用可能になります。  
　レコード番号2には、適用不可能なクライアントの種類を指定します。例えば、10を指定すると、三人打ちと数牌三麻で N/A になります。


### マスク条件 ###

　他の項目の設定次第では意味を失う、または役の成立が不可能になるなどの理由で条件付きのマスクをする場合に使います。次の関数が使えます。

```lua
function chk (ruleTag, itemTag)
    -- ruleTag: ルール項目の英数識別コード
    -- itemTag: ルール設定データ
    return true  -- ruleTagの内容がitemTagと一致している場合
    return false -- そうでない場合
end
```

　例えば、`chk('yakitori', 'no')` の場合、焼き鳥なしルールに設定されていれば `true` が返ります。

　何か入力されていればその値が評価され、 **true の場合は無効** 、false の場合は有効になります。  
　何も入力されていなければ、有効となります。


mjcore/yaku/catalog/sequence.cpp
--------------------------------

　ここで、役の判定方法を実装します。

```c++
    /* ほげほげ */
    if (RuleData::chkRuleApplied("hogehoge"))
        yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
            _T("ほげほげ"), get_yaku_han("hogehoge"),
            _T("ふがふが"), _T("ぴよぴよ"), // ここには下位役を列記する
            [](const MENTSU_ANALYSIS* const analysis) -> bool {
                /* 成立していれば true、していなければ false を返す関数をここに書く */
                return false;
            }
        ));
```

　なお、`get_yaku_han()` 関数は、コンフィグから飜数を取得するのに使います。

### `MentsuAnalysis` 構造体 ###

```c++
struct yaku::yakuCalculator::MentsuAnalysis { // 面子解析結果
    PlayerID player;
    ShantenData shanten;
    MeldBuf MianziDat; // 面子パース結果
    uint8_t BasePoint; // 符
    MachiType Machi; // 待ちの種類
    MachihaiInfo MachiInfo; // 待ち牌の数とか
    bool isPinfu; // 平和になってるかどうか
    Int8ByTile KeziCount; // 刻子・槓子の数
    Int8ByTile AnKeziCount; // 暗刻・暗槓の数
    Int8ByTile DuiziCount; // 対子・刻子・槓子の数
    Int8ByTile ShunziCount; // 順子の数
    Int8ByTile AnShunziCount; // 鳴いていない順子の数
    Int8ByTile KangziCount; // 槓子の数
    Int8ByTile AnKangziCount; // 暗槓の数
    Int8ByTile KaKangziCount; // 加槓の数
    Int8ByTile SeenTiles; // 見えている牌
    Int8ByTile TileCount; // 牌の数
    uint8_t TotalKezi; // 刻子槓子合計
    uint8_t TotalAnKezi; // 暗刻暗槓合計
    uint8_t TotalShunzi; // 順子合計
    uint8_t TotalAnShunzi; // 暗順子合計
    uint8_t TotalKangzi; // 槓子合計
    uint8_t TotalAnKangzi; // 暗槓合計
    uint8_t TotalKaKangzi; // 加槓合計
    const GameTable* GameStat; // 卓情報へのポインタ
    const PlayerTable* PlayerStat() const; // プレイヤー情報へのポインタ
    const Tile& TsumoHai() const; // 和了牌
    bool MenzenFlag() const; // 門前かどうかのフラグ
    bool TsumoAgariFlag() const; // ツモアガリどうかのフラグ
};
```

### `ShantenData` 構造体 ###

```c++
struct yaku::yakuCalculator::ShantenData : public std::array<Shanten, SHANTEN_PAGES> {
    constexpr Shanten operator[] (ShantenType s) const;
    reference operator[] (ShantenType s);
    constexpr Shanten operator[] (std::size_t s) const;
    reference operator[] (std::size_t s);
    ShantenData() = default;
    ShantenData(const ShantenData&) = default;
    ShantenData& operator = (const ShantenData&) = default;
};
```

### `MachiType` 列挙体 ###

```c++
enum class yaku::yakuCalculator::MachiType : uint8_t { // 待ちの種類
    invalid, // 無効
    ryanmen, // 両面
    kanchan, // 嵌張
    penchan, // 辺張
    shanpon, // 双ポン
    tanki    // 単騎
};
```

### `MachihaiInfo` 構造体 ###

```c++
struct MachihaiInfo { // 待ち牌とかの情報を格納(chkFuriten関数用)
    bool FuritenFlag; // フリテンかどうかのフラグ
    InfoByTile<MachihaiTileInfo> Machihai; // 待ち牌情報
    int8_t MachihaiTotal; // 待ち牌の合計枚数
    int8_t MachiMen; // 何面待ち？
};
```

### `InfoByTile` テンプレート ###

```c++
template <class T> class InfoByTile { // 牌ごとに指定した型による情報(テンプレート)
private:
    /* 略 */
public:
    const T& operator[](const TileCode tile) const;
    const T& operator[](const int tile) const;
    T& operator[](const TileCode tile);
    T& operator[](const int tile);
};
```

### `Tile` 構造体 ###

```c++
struct Tile { // 赤ドラデータを含めた牌のデータ
    TileCode tile;
    doraCol red;
    constexpr explicit operator bool() const {return tile != TileCode::noTile;}
    constexpr bool operator ! () const {return tile == TileCode::noTile;}
    constexpr bool operator == (const Tile& otherTile) const;
    constexpr bool operator != (const Tile& otherTile) const;
    constexpr bool operator < (const Tile& otherTile) const;
    constexpr bool operator > (const Tile& otherTile) const;
    constexpr bool operator <= (const Tile& otherTile) const;
    constexpr bool operator >= (const Tile& otherTile) const;
    constexpr TileSuit getSuit() const;
    constexpr bool isNumber() const;
    constexpr bool isHonor() const;
    constexpr bool isFlower() const;
    constexpr explicit Tile(TileCode tile = TileCode::noTile, DoraCol red = DoraCol::normal);
};
```

### `TileCode` 列挙体 ###

```c++
enum class TileCode : uint8_t { // 牌のコード
    /* 萬子 */
    characterOne =   1u, characterTwo, characterThree, characterFour, characterFive, characterSix, characterSeven, characterEight, characterNine,
    /* 筒子 */
    circleOne    =  11u, circleTwo,    circleThree,    circleFour,    circleFive,    circleSix,    circleSeven,    circleEight,    circleNine,
    /* 索子 */
    bambooOne    =  21u, bambooTwo,    bambooThree,    bambooFour,    bambooFive,    bambooSix,    bambooSeven,    bambooEight,    bambooNine,
    /* 字牌 */
    eastWind     =  31u, southWind,    westWind,       northWind,     whiteDragon,   greenDragon,  redDragon,
    /* 花牌 */
    flower       =  38u, // 花牌(InfoByTileのインデックス用)
    spring       = 121u, summer,       autumn,         winter, // 季節牌
    plum         = 126u, orchid,       chrysanthemum,  bamboo, // 草木牌
    /* 以下は役判定で使うことはありません */
    noTile       =   0u, // 牌なし
    backSide     =  39u, // 牌の裏側（画面表示時の内部処理用）
    tilePad      = UCHAR_MAX, // 理牌時の内部処理用
};
```

### `DoraCol` 列挙体 ###

```c++
enum class DoraCol : uint8_t {
    normal, // 黒牌
    akaDora, // 赤牌
    aoDora, // 青牌
};
```

### typedef名 ###

```c++
typedef int8_t PlayerID; // プレイヤー番号
typedef int8_t Shanten; // 向聴数のためのデータ型（0=聴牌、-1=和了、127=無効）
typedef MeldCode MeldBuf[5u];
typedef InfoByTile<int8_t> Int8ByTile;
```

### その他の構造体など ###

　`GameTable` や `PlayerTable` の定義は common/gametbl.h にありますが、状況役でなければ参照することはないでしょう。





非面子役（4面子1雀頭になっていない役）の追加方法
================================================


　非面子役では、言うまでもないことですが、面子解析済みのデータは使えません。また、向聴数の計算方法も異なります。  
　そこでMiHaJongでは、向聴数の計算方法をコードとして記述し、役の判定では、 **その役単独についての向聴数が-1であることを以って役が成立** したものとしています。

　修正するファイルは次の通りです。

- common/shanten.h
- mjcore/data/confitem.csv
- mjcore/shanten.h
- mjcore/shanten.cpp
- mjcore/yaku/catalog/irreg.cpp
- mjcore/ai/functbl.cpp
- doc/script.txt

　ここでは、「ふがほげ」という非面子役を追加するとします。


common/shanten.h
----------------

　ここでは、`ShantenType` 列挙体に定数を追加します。`pages,` と書いてある行のすぐ上に次のように追加します。

```c++
    fugahoge, // ふがほげ
```


mjcore/data/confitem.csv
------------------------

　ここでは、レコードを追加します。方法は面子役の場合と同じです。


mjcore/shanten.h
----------------

　ここでは、`ShantenAnalyzer` クラスに private メソッドを追加します。`ShantenAnalyzer` クラスはモノステートです。

```c++
    static Shanten calcShantenFugahoge(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount); 
```


mjcore/shanten.cpp
------------------

　ここですることは大きく分けて2つあります。

### `ShantenAnalyzer::calcShanten()` の引数ごとの分岐 ###

　次の2箇所を書き換えます。

- 引数に `ShantenType::fugahoge` が渡された場合の処理

　これは、「ふがほげ」に対する向聴数を求めよということです。`switch (mode)` のブロック内に、次のように追記します。

```c++
case ShantenType::fugahoge:
    return calcShantenFugahoge(gameStat, playerID, tileCount);
```

- 引数に `ShantenType::all` が渡された場合の処理

　これは、「ふがほげ」の他に面子手や七対子など可能なすべての和了形に対する向聴数を求めよということです。`default` ケース内で、 `return shanten;` と書かれている前の行に、次のように追加します。

```c++
tmpShanten = calcShantenFugahoge(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten; 
```

### `ShantenAnalyzer::calcShantenFugahoge()` の実装 ###

　`ShantenAnalyzer::calcShantenFugahoge()` を実装します。

```c++
    Shanten ShantenAnalyzer::calcShantenFugahoge(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount)
    { // 特殊：ふがほげの向聴数を求める
        if (!RuleData::chkRuleApplied("fugahoge")) return ShantenImpossible;

        Shanten shanten = 13;
        /* ここに計算処理を書く */

        return shanten;
    }
```


mjcore/yaku/catalog/irreg.cpp
-----------------------------

　ここでは、`yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_irregular()` 関数内に追記します。

```c++
    /* ふがほげ */
    if (RuleData::chkRuleApplied("fugahoge"))
        yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
            _T("ふがほげ"), get_yaku_han("fugahoge"),
            [](const MENTSU_ANALYSIS* const analysis) -> bool {
                return (analysis->shanten[ShantenType::fugahoge] == -1);
            }
        ));
```


mjcore/ai/functbl.cpp
---------------------

　ここでは、AIから「ふがほげ」の飜数を取得できるように、`aiscript::table::functable::agariTypeCode()` に追記します。

```c++
    lockTable(L); lua_setfield(L, -2, "AgariType");
```

と書かれている前の行に、次のように追加します。

```c++
    TableAdd(L, "Fugahoge", (lua_Integer)ShantenType::fugahoge);
```


doc/script.md
-------------

　doc/script.md には、AI用のスクリプトの説明が書かれています。750～760行目辺りにそれらしい解説があるので、そこにも忘れず追記してください。


