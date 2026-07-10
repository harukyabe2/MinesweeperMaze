## マインスイーパー迷路<br>

# 動画<br>
https://youtu.be/fUZvUbDef1w

# 利用ツール<br>
<img src="https://img.shields.io/badge/-C++-00599C.svg?logo=c%2B%2B&style=flat-square&logoColor=white" alt="C++"> <img src="https://img.shields.io/badge/-Siv3D-00A4E8.svg?style=flat-square&logoColor=white" alt="Siv3D"><br>
ゲームの実行中に**F1**キーを押すことでSiv3Dのライセンス情報を表示できます

**アセット利用情報**<br>
なし
<br>

# 操作説明<br>
**左クリック**でマスを開ける<br>
**右クリック**で旗を立てる・取り消す<br>
ゲームオーバー時には**C**キーでコンティニュー，**E**キーで終了
<br>

# 制作<br>
**個人制作**<br>
制作期間：3日
<br>

# 作品のコンセプト<br>
マインスイーパーです．マスを開けたときに出る数字を参考にして，地雷を避けながらすべてのマスを開けるゲームです．
<br>

# 設計と機能説明<br>
**技術的なアピールポイント**<br>
・各マスの状態は外から直接操作せず，親クラスであるBoardクラスを経由してやりとり<br>
・追加ステージの作成やギミック追加がしやすくなるようにし，拡張性を持たせた<br>

**クラス説明**<br>

**Gameクラス**<br>
ゲーム全体の心臓部であり，アセットの初期化，入力処理，描画を統括する．
<br>
**Boardクラス**<br>
マス目の状態を管理し，地雷の配置や開いたマスの状態を更新する．
<br>
**Cellクラス**<br>
各マスの状態を保持し，描画やフラグの管理を行う．
<br>

# 動作環境<br>
**Windows**<br>

**対応OS**: Windows 10 / 11<br>

**実行方法**<br>
Releasesから**Minesweeper.zip**をダウンロードし，解凍します<br>
解凍してできた**Minesweeper.exe**を実行するとゲームが始まります<br>
セキュリティ機能によって開けない場合は詳細情報から実行を選択してください<br>
<br>
