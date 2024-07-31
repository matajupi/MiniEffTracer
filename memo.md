# TODO:
- PEffectを実装 & Bind
- PPrimFunとPFun、PEffectはすべてPCallableというInterfaceを実装する
    - PCallable::Call(Tracer \*tracer);

- with-handleの実装:
    - Handler部を評価して、PHandlerを得る
    - 新しいenvを作って、POpEffのeff部とGeneric effectをバインド
    - Body部を評価する
    - Body部でeffectが呼び出されたら、Generic effectを外側に流していく(イミュータブルに)
        - コンパイラ作成ではもっと良い処理方法がある?
        - それぞれの構成要素で条件分岐(もっと良い書き方?)
    - Body部からPEffectが得られて、handlerが処理できれば対応するOperation caseで処理
    - handlerが処理できなければ、外側に流す
    - Body部からValueが得られたら、Return caseで処理

- Op call(Generic effect)の実装:
    - with ~~~ handle ~~~ opc x ~~~ -> with ~~~ handle eff x (y.~~~ y ~~~)
    - f (op x (z.A)) => op x (z.f A)
    - (op x (z.A)) y => op x (z.A y)
    - let x = 3 in op x (z.A) => op x (z.A) [x: 3]
    - A; op x (z.C); B => A; op x (y.C; B)
    - if op x (z.C) then A else B => op x (z.if C then A else B)
    - if true then op1 x (z.A) else op2 x (z.B) => op1 x (z.A)
    - if false then op1 x (z.A) else op2 x (z.B) => op2 x (z.B)
        - if-then-elseは分岐後のStatementだけをBind(conditionにop callがあったら数回呼び出される)
    - let f x = if x = 0 then op1 0 (z.A) else op2 (f (x - 1)) (z.B) in f 1
                => op2 (f 0) (z.B) [f: <fun>] => op2 (op1 0 (z.A)) (z.B)
                => op1 0 (z.op2 A (z.B))
    - op1 (op2 x) => let y = op2 x in op1 y
                  => op2 x (z.let y = z in op1 y)
                  => op2 x (z.op1 z)
        - op1とop2が逆転(ref: SemanticsにContinuationを導入すると、関数の結合表記が逆転する話)
    - op callのenvを引き連れる

let a = 3
in
    let b =
        let c = 4
        in
            let d = op (a + c)
            in d
    in
        b
=>
let a = 3
in
    let b =
        let c = 4
        in
            op (a + c) (z.let d = z in d)
    in
        b
=>
let a = 3
in
    let b = op (a + c) (z.let d = z in d)
    in
        b
{c: 4}
=>
let a = 3
in
    op (a + c) (z.let b = (let d = z in d) in b)
{c: 4}
=>
op (a + c) (z.
    let b = 
        let d = z
        in d
    in b)
{a: 3, c: 4}


- TODO: NFun, NHandlerからPFun, PHandlerが作れるようにする
    -> Visitをポインタ渡しにする
- TODO: PFun, PPrimFun, POpCをPCallableで抽象化
- TODO: Toplevel handler

- BNF:
prog := defs

defs := ""
    | defs def ";;"

def := topexpr
    | "let" ident "=" topexpr
    | "let" "rec" ident "=" topexpr
    | "let" ident ident "=" topexpr
    | "let" "rec" ident ident "=" topexpr

topexpr := letexpr
    | topexpr ";" topexpr (right assoc)

letexpr := arithexpr
    | "fun" ident "->" topexpr
    | "let" ident "=" topexpr "in" topexpr
    | "let" "rec" ident "=" topexpr "in" topexpr
    | "let" ident ident "=" topexpr "in" topexpr
    | "let" "rec" ident ident "=" topexpr "in" topexpr
    | "if" topexpr "then" topexpr "else" letexpr
    | "handler" opcs
    | "with" topexpr "handle" topexpr

opcs := opcs "|" opcs (right assoc)
    | ""
    | ident "->" topexpr
    | ident ident ident "->" topexpr

arithexpr := appexpr
    | arithexpr "=" arithexpr (left assoc)
    | arighexpr "<" arithexpr (left assoc)
    | arighexpr ">" arithexpr (left assoc)
    | arighexpr "+" arithexpr (left assoc)
    | arighexpr "-" arithexpr (left assoc)
    | arighexpr "*" arithexpr (left assoc)
    | arighexpr "/" arithexpr (left assoc)

appexpr := btexpr
    | appexpr btexpr

btexpr := "true"
    | "false"
    | number
    | ident
    | "(" ")"
    | "(" topexpr "," topexpr ")"
    | "(" topexpr ")"

