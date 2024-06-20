# TODO:
- 様々なContextやConfigurationを持ったCustom Tracerを扱えるようにする
    - Visitorパターンを採用
- ASTをconstにする
    - Tracer classの内部で計算状態を保持
    - Custom TracerによってStepの大きさを変えられるようにする
