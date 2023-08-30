{-# OPTIONS_GHC -Wall #-}
{-# LANGUAGE EmptyDataDecls, MultiParamTypeClasses,
             TypeSynonymInstances, FlexibleInstances,
             InstanceSigs #-}

module Basics where
{-
    Expune funcțiile necesare reprezentării jocului.
-}

import ProblemState
import Data.List
import qualified Data.Array as Array

{-
    Sinonim tip de date pentru reprezetarea unei perechi (Int, Int)
    care va reține coordonatele celulelor de pe tabla de joc.
    Colțul stânga-sus este (0, 0).
-}
type Position = (Int, Int)

{-
    Tip de date pentru reprezentarea Target-urilor.
    Acestea conțin informații atât despre poziția curentă a
    Target-ului cât și despre comportamentul acestuia.
    Tipul Behavior este definit mai jos.
-}
data Target = Target {
    position :: Position,
    behavior :: Behavior
}

instance Eq Target where
    Target p1 _ == Target p2 _ = p1 == p2

instance Ord Target where
    Target p1 _ <= Target p2 _ = p1 <= p2

{-
    Tip de date pentru reprezentarea comportamentului unui Target.
    Tipul Behavior este utilizat pentru a modela tranziția Target-urilor
    din starea curentă în starea următoare. Primul parametru este poziția
    actuală a target-ului, iar al doilea, starea curentă a jocului.
    Tipul Game este definit mai jos.
    
    Observați că, din moment ce un Behavior produce un Target nou,
    acesta din urmă ar putea fi caracterizat de un alt Behavior
    decât cel anterior.
-}
type Behavior = Position -> Game -> Target

{-
    Direcțiile de deplasare pe tablă
-}
data Direction = North | South | West | East
    deriving (Eq, Show)

{-
    *** TODO ***
    
    Tip de date pentru reprezentarea stării jocului, la un anumit
    moment. Completați-l cu orice informație aveți nevoie pentru
    stocarea stării jocului (hunter, target, obstacole, gateways).
-}

data Cell = HunterC | TargetC | ObstacleC | GatewayC | BlankC
    deriving (Eq, Ord)

instance Show Cell
    where
        show HunterC = "!"
        show TargetC = "*"
        show ObstacleC = "@"
        show GatewayC = "#"
        show BlankC = " "

data Game = Game {
    board :: Array.Array Position [Cell],
    boardSize :: (Int, Int),
    gateways :: [(Position, Position)],
    targets :: [Target],
    hunter :: Position
} deriving (Eq, Ord)
{-
    *** Optional *** 
  
    Dacă aveți nevoie de o funcționalitate particulară,
    instantiați explicit clasele Eq și Ord pentru Game.
    În cazul acesta, eliminați deriving (Eq, Ord) din Game.
-}

{-
    *** TODO ***

    Reprezentați starea jocului ca șir de caractere, pentru afișarea
    la consolă.
    
    Atenție! Fiecare linie, mai puțin ultima, este urmată de \n.
    Celule goale vor fi reprezentate ca ' '.
    Hunter-ul va fi reprezentat ca '!'.
    Target-urile vor fi reprezentate ca '*'
    Gateways-urile vor fi reprezentate ca '#'.
    Obstacolele vor fi reprezentate de '@'.

    Hint: S-ar putea să vă fie utile list comprehensions,
    precum și funcțiile elem, any și intercalate din Data.List.
-}
gameAsString :: Game -> String
gameAsString = show

instance Show Game
    where
        show (Game b (lin, col) _ _ _) = 
            filter (not . (`elem` "[,]" )) $ intercalate "\n" 
            [concat [show $ head $ b Array.! (i, j) | j <- [0..col - 1]] | i <- [0..lin - 1]]

{-
    *** TODO ***
    
    Primește numărul de linii și numărul de coloane ale tablei de joc.
    Intoarce un obiect de tip Game în care tabla conține spații goale în interior, fiind
    împrejmuită de obstacole pe toate laturile. Implicit, colțul din stânga sus este (0,0),
    iar Hunterul se găsește pe poziția (1, 1).
-}
emptyGame :: Int -> Int -> Game
emptyGame lin col = Game b (lin, col) [] [] (1, 1)
    where 
        b = Array.array ((0, 0), (lin - 1, col - 1))
                    ([((i, j), [ObstacleC]) | i <- [0, lin - 1], j <- [0..col - 1]] ++
                    [((i, j), [ObstacleC]) | i <- [1..lin - 2], j <- [0, col - 1]] ++
                    [((i, j), [BlankC]) | i <- [1..lin - 2], j <- [1..col - 2]] ++
                    [((1, 1), [HunterC])])

removeACell :: Position->Cell->Game->[Cell]
removeACell pos cell (Game b _ _ _ _)
    | b Array.! pos == [BlankC] = [BlankC]
    | b Array.! pos == [cell] = [BlankC]
    | elem cell $ b Array.! pos = tail $ b Array.! pos
    | otherwise = b Array.! pos

addACell :: Position->Cell->Game->[Cell]
addACell pos cell (Game b _ _ _ _)
    | b Array.! pos == [TargetC] = case cell of
        TargetC -> cell : b Array.! pos
        _ -> b Array.! pos
    | b Array.! pos == [GatewayC] = case cell of
        TargetC -> cell : b Array.! pos
        HunterC -> cell : b Array.! pos
        _ -> b Array.! pos
    | b Array.! pos == [BlankC] = [cell]
    | otherwise = b Array.! pos

{-
    *** TODO ***

    Primește o poziție și un joc și întoarce un nou joc, cu Hunter-ul pus
    pe poziția specificată.
    Parametrul Position reprezintă poziția de pe hartă la care va fi adaugat Hunter-ul
    Daca poziția este invalidă (ocupată sau în afara tablei de joc) se va întoarce
    același joc.
-}


addHunter :: Position -> Game -> Game
addHunter pos@(x, y) game@(Game b (lin, col) g t hT)
    | x >= lin || x < 0 = game 
    | y >= col || y < 0 = game
    | b Array.! (x, y) /= [BlankC] = game
    | b Array.! hT /= [HunterC] = Game newBoard1 (lin, col) g t pos
    | otherwise = Game newBoard2 (lin, col) g t pos
        where
            newBoard1 = b Array.// [(pos, [HunterC])]
            newBoard2 = (b Array.// [(pos, [HunterC])]) Array.// [(hT, [BlankC])]

{-
    *** TODO ***

    Primește un comportament, o poziție și un joc și întoarce un nou joc, în care a fost
    adăugat Target-ul descris de comportament și poziție.
    Parametrul Behavior reprezintă comportamentul Hunter-ului care va fi adăugat.
    Parametrul Position reprezintă poziția de pe hartă la care va fi adăugat Target-ul.
-}

addTarget :: Behavior -> Position -> Game -> Game
addTarget behv pos (Game b (lin, col) g t hT) = Game newBoard (lin, col) g (t ++ [newT]) hT
    where
        newBoard = b Array.// [(pos, [TargetC])]
        newT = Target pos behv 
{-
    *** TODO ***

    Primește o pereche de poziții și un joc și întoarce un nou joc, în care au fost adăugate
    cele două gateway-uri interconectate.
    Parametrul (Position, Position) reprezintă pozițiile de pe hartă la care vor fi adăugate 
    cele două gateway-uri interconectate printr-un canal bidirecțional.
-}
addGateway :: (Position, Position) -> Game -> Game
addGateway gPos@(posI, posO) (Game b (lin, col) g t hT) = Game newBoard (lin, col) (g ++ [gPos]) t hT
    where
        newBoard = (b Array.// [(posI, [GatewayC])]) Array.// [(posO, [GatewayC])]
        

{-
    *** TODO ***

    Primește o poziție și un joc și întoarce un nou joc, în care a fost adăugat un obstacol
    la poziția specificată.
    Parametrul Position reprezintă poziția de pe hartă la care va fi adăugat obstacolul.
-}
addObstacle :: Position -> Game -> Game
addObstacle pos (Game b (lin, col) g t hT) = Game newBoard (lin, col) g t hT
    where
        newBoard = b Array.// [(pos, [ObstacleC])]

{-
    *** TODO ***
    
    Primește o poziție destinație înspre care vrea să se deplaseze o entitate (Hunter sau Target)
    și verifică daca deplasarea este posibilă, întorcând noua poziție, luând în considerare
    și Gateway-urile.
    Avem următoarele cazuri:
    - dacă poziția corespunde unui spațiu gol, se întoarce acea poziție;
    - dacă poziția corespunde unui gateway, se întoarce poziția gateway-ului pereche;
    - dacă poziția corespunde unui obstacol, se întoarce Nothing.
    Parametrul Position reprezintă poziția destinație.
-}
gatewayPair :: Position -> [(Position, Position)] -> Position
gatewayPair pos g = head list
    where 
        list = case [posO | (posI, posO) <- g, posI == pos] of
                        [] -> [posI | (posI, posO) <- g, posO == pos]
                        _ -> [posO | (posI, posO) <- g, posI == pos]

attemptMove :: Position -> Game -> Maybe Position
attemptMove pos (Game b _ g _ _)
    | b Array.! pos == [BlankC] = Just pos
    | b Array.! pos == [GatewayC] = Just (gatewayPair pos g)
    | otherwise = Nothing

{-
    *** TODO ***

    Comportamentul unui Target de a se deplasa cu o casuță înspre est. 
    Miscarea se poate face doar daca poziția este validă (se află pe tabla de
    joc) și nu este ocupată de un obstacol. In caz contrar, Target-ul va rămâne 
    pe loc.
    
    Conform definiției, tipul Behavior corespunde tipului funcție
    Position -> Game -> Target.
    
    Având în vedere că cele patru funcții definite în continuare (goEast, goWest,
    goNorth, goSouth) sunt foarte similare, încercați să implementați o funcție
    mai generală, pe baza căreia să le definiți apoi pe acestea patru.
-}

goSomewhere :: Direction -> Behavior
goSomewhere dir posCurr@(x, y) (Game b _ g _ _)
    | b Array.! posNext == [BlankC] = (Target posNext (goSomewhere dir))
    | b Array.! posNext == [GatewayC] = (Target (gatewayPair posNext g) (goSomewhere dir))
    | b Array.! posNext == [ObstacleC] = case b Array.! (x, y) of
        TargetC : [GatewayC] -> (Target (gatewayPair posCurr g) (goSomewhere dir))
        _ -> (Target posCurr (goSomewhere dir))
    | otherwise = (Target posCurr (goSomewhere dir))
        where
            posNext = case dir of
                North -> (x - 1, y)
                South -> (x + 1, y)
                East -> (x, y + 1)
                West -> (x, y -1)


goEast :: Behavior
goEast = goSomewhere East

{-
    *** TODO ***

    Comportamentul unui Target de a se deplasa cu o casuță înspre vest. 
    Miscarea se poate face doar daca poziția este validă (se află pe tabla de
    joc) și nu este ocupată de un obstacol. In caz contrar, Target-ul va rămâne 
    pe loc.
-}

goWest :: Behavior
goWest = goSomewhere West

{-
    *** TODO ***

    Comportamentul unui Target de a se deplasa cu o casuță înspre nord. 
    Miscarea se poate face doar daca poziția este validă (se află pe tabla de
    joc) și nu este ocupată de un obstacol. In caz contrar, Target-ul va rămâne 
    pe loc.
-}
goNorth :: Behavior
goNorth = goSomewhere North

{-
    *** TODO ***

    Comportamentul unui Target de a se deplasa cu o casuță înspre sud. 
    Miscarea se poate face doar daca poziția este validă (se află pe tabla de
    joc) și nu este ocupată de un obstacol. In caz contrar, Target-ul va rămâne 
    pe loc.
-}
goSouth :: Behavior
goSouth = goSomewhere South

{-
    *** TODO ***

    Comportamentul unui Target de a-și oscila mișcarea, când înspre nord, când înspre sud. 
    Mișcarea se poate face doar dacă poziția este validă (se află pe tablă de
    joc) și nu este ocupată de un obstacol. In caz contrar, Target-ul iși va schimba
    direcția de mers astfel:
    - daca mergea inspre nord, își va modifica direcția miscării înspre sud;
    - daca mergea inspre sud, își va continua mișcarea înspre nord.
    Daca Target-ul întâlneste un Gateway pe traseul său, va trece prin acesta,
    către Gateway-ul pereche conectat și își va continua mișcarea în același sens la ieșire
    din acesta.
    Puteți folosit parametrul Int pentru a surprinde deplasamentul Target-ului (de exemplu,
    1 pentru sud, -1 pentru nord).
-}
bounce :: Int -> Behavior
bounce inT pos@(x, y) (Game b _ g _ _)
    | b Array.! posNew == [BlankC] = case inT of
        1 -> (Target (x + 1, y) (bounce 1))
        _ -> (Target (x - 1, y) (bounce (-1)))
    | b Array.! posNew == [ObstacleC] = case inT of
        1 -> (Target (x - 1, y) (bounce (-1)))
        _ -> (Target (x + 1, y) (bounce 1))
    | b Array.! posNew == [GatewayC] = (Target (gatewayPair posNew g) goWhere)
    | otherwise = (Target pos goWhere)
        where
            posNew = case inT of
                1 -> (x + 1, y)
                _ -> (x - 1, y)
            goWhere = case inT of
                1 -> (bounce 1)
                _ -> (bounce (-1))

{-
    *** TODO ***
    Funcție care mută toate Target-urile din Game-ul dat o poziție, în functie
    de behavior-ul fiecăreia și întoarce noul Game în care pozițiile Target-urilor
    sunt actualizate.
    
-}

moveTargets :: Game -> Game
moveTargets game@(Game b (lin, col) g t hT) = (Game newBoard (lin, col) g newT hT)
    where
        newT = [behv pos game | (Target pos behv) <- t]
        newBoard = board2 Array.// [(pos, addACell pos TargetC game) | (Target pos _) <- newT]
            where
                board2 = b Array.// [(pos, removeACell pos TargetC game) | (Target pos _) <- t]

{-
    *** TODO ***

    Verifică dacă Targetul va fi eliminat de Hunter.
    Un Target este eliminat de Hunter daca se află pe o poziție adiacentă
    cu acesta.
    Parametrul Position reprezintă poziția Hunterului pe tabla
    de joc.
    Parametrul Target reprezintă Targetul pentru care se face verificarea.
-}
isTargetKilled :: Position -> Target -> Bool
isTargetKilled (xH, yH) (Target (xT, yT) _)
    | xT == xH + 1 && yT == yH = True
    | xT == xH - 1 && yT == yH = True
    | xT == xH && yT == yH + 1 = True
    | xT == xH && yT == yH - 1 = True
    | otherwise = False

{-
    *** TODO ***

    Avansează starea jocului curent, rezultând starea următoare a jocului.
    Parametrul Direction reprezintă direcția în care se va deplasa Hunter-ul.
    Parametrul Bool specifică dacă, după mutarea Hunter-ului, vor fi
    mutate și Target-urile sau nu, și dacă vor fi eliminate din joc sau nu.
    Este folosit pentru a distinge între desfășurarea reală a jocului (True)
    și planificarea „imaginată” de hunter (False) în partea a doua a temei.

    Avansarea stării jocului respectă următoarea ordine:
    1. Se deplasează Hunter-ul.
    2. În funcție de parametrul Bool, se elimină Target-urile omorâte de către Hunter.
    3. In funcție de parametrul Bool, se deplasează Target-urile rămase pe tablă.
    4. Se elimină Targeturile omorâte de către Hunter și după deplasarea acestora.
    
    Dubla verificare a anihilării Target-urilor, în pașii 2 și 4, îi oferă Hunter-ului
    un avantaj în prinderea lor.
-}

getPosH :: Direction -> Position -> Position
getPosH direction (xH, yH)
    | direction == North = (xH - 1, yH)
    | direction == South = (xH + 1, yH)
    | direction == East = (xH, yH + 1)
    | otherwise = (xH, yH - 1)


moveHunter :: Position -> Game -> Game
moveHunter pos game@(Game b (lin, col) g t hT)
    | b Array.! pos == [BlankC] = Game newBoard1 (lin, col) g t pos
    | b Array.! pos == [GatewayC] = Game newBoard2 (lin, col) g t newPos
    | otherwise = game
        where
            newPos = (gatewayPair pos g)
            newBoard1 = (b Array.// [(pos, [HunterC])]) Array.// [(hT, removeACell hT HunterC game)]
            newBoard2 = (b Array.// [(newPos, addACell newPos HunterC game)]) Array.// [(hT, removeACell hT HunterC game)]

firstMove :: Direction -> Bool -> Game -> Game
firstMove direction kill game@(Game _ (lin, col) g t hT) = case kill of
    True -> moveTargets $ Game newBoard (lin, col) g newT hN
    False -> newGame
    where
        newGame@(Game boardN _ _ tN hN) = moveHunter newH game
        newH = getPosH direction hT
        newT = [target | target <- tN, not (isTargetKilled hN target)]
        newBoard = boardN Array.// [(pos, removeACell pos TargetC newGame) |
            target@(Target pos _) <- t, (isTargetKilled hN target)]

advanceGameState :: Direction -> Bool -> Game -> Game
advanceGameState direction kill game@(Game _ _ g _ _) = case kill of
    True -> Game newBoard (linN, colN) g newT hN
    False -> firstMove direction kill game
    where
        newGame@(Game boardN (linN, colN) _ tN hN) = firstMove direction kill game
        newT = [target | target <- tN, not (isTargetKilled hN target)]
        newBoard = boardN Array.// [(pos, removeACell pos TargetC newGame) |
            target@(Target pos _) <- tN, (isTargetKilled hN target)]
{-
    ***  TODO ***

    Verifică dacă mai există Target-uri pe table de joc.
-}
areTargetsLeft :: Game -> Bool
areTargetsLeft (Game _ _ _ t _)
    | t == [] = False
    | otherwise = True

{-
    *** BONUS TODO ***

    Comportamentul unui Target de a se deplasa în cerc, în jurul unui Position, având
    o rază fixată.
    Primul parametru, Position, reprezintă centrul cercului.
    Parametrul Int reprezintă raza cercului.
    Puteți testa utilizând terenul circle.txt din directorul terrains, în conjuncție
    cu funcția interactive.
-}
circle :: Position -> Int -> Behavior
circle = undefined


instance ProblemState Game Direction where
    {-
        *** TODO ***
        
        Generează succesorii stării curente a jocului.
        Utilizați advanceGameState, cu parametrul Bool ales corespunzător.
    -}
    successors currentState = [(action, advanceGameState action False currentState) | action <- [North, South, East, West]]

    {-
        *** TODO ***
        
        Verifică dacă starea curentă este un în care Hunter-ul poate anihila
        un Target. Puteți alege Target-ul cum doriți, în prezența mai multora.
    -}
    isGoal (Game _ _ _ t hT) = (length [target | target <- t, isTargetKilled hT target]) > 0

    {-
        *** TODO ***
        
        Euristica euclidiană (vezi hEuclidian mai jos) până la Target-ul ales
        de isGoal.
    -}
    h currentState@(Game _ _ _ t hT)
        | isGoal currentState == True = hEuclidean posT hT
        | otherwise = 0
        where
            (Target posT _) = head [target | target <- t, isTargetKilled hT target]
{-
     ** NU MODIFICATI **
-}
hEuclidean :: Position -> Position -> Float
hEuclidean (x1, y1) (x2, y2) = fromIntegral $ ((x1 - x2) ^ pow) + ((y1 - y2) ^ pow)
  where
    pow = 2 :: Int

{-
    *** BONUS ***

    Acesta reprezintă un artificiu necesar pentru testarea bonusului,
    deoarece nu pot exista două instanțe diferite ale aceleiași clase
    pentru același tip.

    OBSERVAȚIE: Testarea bonusului pentru Seach este făcută separat.
-}

newtype BonusGame = BonusGame Game
    deriving (Eq, Ord, Show)

{-
    *** BONUS TODO ***

    Folosind wrapper-ul peste tipul Game de mai sus instanțiați
    ProblemState astfel încât să fie folosită noua euristică. 
-}
instance ProblemState BonusGame Direction where
    {-
        *** BONUS TODO ***

        Pentru a ne asigura că toțî succesorii unei stări sunt de tipul
        BonusGame și folosesc noua euristică trebuie să aplicăm wrapper-ul
        definit mai sus peste toți succesorii unei stări.

        Hint: Puteți să folosiți funcția fmap pe perechi pentru acest lucru.
        https://wiki.haskell.org/Functor
    -}
    successors = undefined

    {-
        *** BONUS TODO ***

        Definiți funcția isGoal pentru BonusGame.

        Hint: Folosiți funcția isGoal deja implementată pentru tipul Game.
    -}
    isGoal = undefined

    {-
        *** BONUS TODO ***

        Definiți o funcție euristică care este capabilă să găsească un drum mai scurt
        comparativ cu cel găsit de euristica implementată pentru Game.

        ATENȚIE: Noua euristică NU trebuie să fie una trivială.

        OBSERVAȚIE: Pentru testare se va folosi fișierul terrains/game-6.txt.
    -}
    h = undefined
