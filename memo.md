# TODO:
- PEffectを実装 & Bind
- PPrimFunとPFun、PEffectはすべてPCallableというInterfaceを実装する
    - PCallable::Call(Tracer \*tracer);

- handlerの実装:
    - OpCs部を評価して、それぞれPOpRet, POpEffを得る
    - POpはPFunなどと同じように、中身には立ち入らない
    - PHandlerに格納して返す

- with-handleの実装:
    - Handler部を評価して、PHandlerを得る
    - HandlerのRet以外のPOpEffを環境にバインドする
    - Body部を評価する
    - Body部でPOpEffが呼び出されたら、continuationを作り、PEffectを返す
    - handlerがPEffectを処理できれば...
    - handlerがPEffectを処理できなければ...
    - PEffect以外が帰ってきたら、POpRetによるPEffectを返す

- TODO: NFun, NHandlerからPFun, PHandlerが作れるようにする
    -> Visitをポインタ渡しにする
- TODO: PFun, PPrimFun, POpCをPCallableで抽象化
