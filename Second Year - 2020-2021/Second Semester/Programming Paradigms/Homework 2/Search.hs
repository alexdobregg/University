{-# OPTIONS_GHC -Wall #-}
{-# LANGUAGE MultiParamTypeClasses #-}

module Search where

import ProblemState
import qualified Data.PSQueue as PQ
import Data.Maybe
import Prelude
import qualified Data.Set as S

{-
    *** TODO ***
    Tipul unei nod utilizat în procesul de căutare. Recomandăm reținerea unor
    informații legate de:
    * stare;
    * acțiunea care a condus la această stare;
    * nodul părinte, prin explorarea căruia a fost obținut nodul curent;
    * adâncime;
    * estimarea costului până la starea finală;
    * copiii, ce vor desemna stările învecinate;
-}

data Node s a = Nod {state :: s, action :: Maybe a, parent :: Maybe (Node s a), depth :: Int, estCost :: Float, children :: [Node s a]}

{-
    *** TODO ***
    Instanțiați Eq și Ord pe baza stării.
-}

instance Eq s => Eq (Node s a) where
    Nod s1 _ _ _ _ _ == Nod s2 _ _ _ _ _ = s1 == s2

instance Ord s => Ord (Node s a) where
    Nod s1 _ _ _ _ _ <= Nod s2 _ _ _ _ _ = s1 <= s2

{-
    *** TODO ***
    Gettere folosite pentru accesul la câmpurile nodului
-}

nodeState :: Node s a -> s
nodeState (Nod s _ _ _ _ _) = s

nodeParent :: Node s a -> Maybe (Node s a)
nodeParent (Nod _ _ p _ _ _) = p

nodeDepth :: Node s a -> Int
nodeDepth (Nod _ _ _ d _ _) = d

nodeChildren :: Node s a -> [Node s a]
nodeChildren (Nod _ _ _ _ _ c) = c

nodeHeuristic :: Node s a -> Float
nodeHeuristic (Nod _ _ _ d eC _) = eC - fromIntegral d 

nodeAction :: Node s a -> Maybe a
nodeAction (Nod _ a _ _ _ _) = a

{-
    *** TODO ***
    Generarea întregului spațiu al stărilor.
    Primește starea inițială și creează nodul corespunzător acestei stări,
    având drept copii nodurile succesorilor stării curente, și așa mai
    departe, recursiv.
-}

createStateSpaceHelper :: (ProblemState s a, Eq s) => s -> a -> Node s a -> Int -> Node s a
createStateSpaceHelper s a p d = nodCurr
    where
        nodCurr = Nod s (Just a) (Just p) d ((h s) + fromIntegral d) succs
        succs = [createStateSpaceHelper nextS act nodCurr (d + 1) | (act, nextS) <- successors s]

createStateSpace :: (ProblemState s a, Eq s) => s -> Node s a
createStateSpace initialState = nodCurr
    where
        nodCurr = Nod initialState Nothing Nothing 0 (h initialState) succs
        succs = [createStateSpaceHelper nextS act nodCurr 1 | (act, nextS) <- successors initialState]

{-
    Funcție ce primește o coadă de priorități și întoarce o pereche
    formată din cheia cu prioritatea minimă și coada din care a fost ștearsă
    aceasta.
    Hint: O puteți folosi pentru a extrage și a șterge un nod din frontieră.
-}

deleteFindMin :: (Ord k, Ord p) => (PQ.PSQ k p) -> (k, (PQ.PSQ k p))
deleteFindMin pq = (minK, pq')
    where minK = PQ.key $ fromJust $ PQ.findMin pq
          pq' = PQ.deleteMin pq

{-
    *** TODO ***
    Primește nodul curent și mulțimea stărilor vizitate și întoarce
    o listă cu nodurile succesor nevizitate, care ar putea fi introduse
    în frontieră.
-}

suitableSuccs :: (ProblemState s a, Ord s) => Node s a -> (S.Set s) -> [Node s a]
suitableSuccs node visited = [childNode | childNode <- (nodeChildren node), not $ S.member (nodeState childNode) visited]

{-
    *** TODO ***
    Primește o frontieră (o coadă de priorități) și un nod ce trebuie inserat în aceasta,
    întorcând o nouă frontieră.
    ATENȚIE: Dacă la introducerea unui nod există deja în frontieră un alt nod cu aceeași
    stare, dar cu cost mai mare, nodul nou, cu cost mai mic îl va înlocui pe cel vechi.
    
    Hints:
    1. Vedeți funcția insertWith din pachetul PSQueue.
        (https://hackage.haskell.org/package/PSQueue-1.1.0.1/docs/Data-PSQueue.html#v:insertWith)
    2. Costul se calculează ca suma dintre adâncime și euristică.
-}

insertSucc :: (ProblemState s a, Ord s) => (PQ.PSQ (Node s a) Float) -> Node s a -> PQ.PSQ (Node s a) Float
insertSucc frontier node = case PQ.lookup node frontier of
    Nothing -> PQ.insert node costNode frontier
    Just cost -> case cost > costNode of
        True -> PQ.insert node costNode (PQ.delete node frontier)
        False -> frontier 
    where
        costNode = (nodeHeuristic node) + fromIntegral (nodeDepth node)
{-
    *** TODO ***
    Primește nodul curent, frontiera și mulțimea stărilor vizitate, întorcând noua
    frontieră (coadă de priorități) în care au fost adăugate nodurile succesor validate
    de suitableSuccs.
-}

insertSuccs :: (ProblemState s a, Ord s) => (Node s a) -> (PQ.PSQ (Node s a) Float) -> (S.Set s) -> (PQ.PSQ (Node s a) Float)
insertSuccs node frontier visited = foldl (\ front nod -> insertSucc front nod) frontier (suitableSuccs node visited)

{-
    *** TODO ***
    Funcție helper care implementează A-star.
    Primește o mulțime de noduri vizitate și o coadă de priorități (aka frontiera) și
    întoarce starea finală.
    Se procedează astfel până la întâlnirea unei stări scop:
        - se extrage un nod adecvat din frontireră
        - se marchează starea acestuia ca fiind vizitată
        - se introduc succesorii în frontieră
-}

astar' :: (ProblemState s a, Ord s) => (S.Set s) -> (PQ.PSQ (Node s a) Float) -> Node s a
astar' visited frontier = case isGoal s of
    True -> goalNode
    False -> astar' (S.insert s visited) (insertSuccs goalNode auxFront (S.insert s visited))
    where
        (goalNode, auxFront) = deleteFindMin frontier
        s = nodeState goalNode

{-
    *** TODO ***
  
    Primește starea inițială și întoarce starea finală pentru o singură aplicare
    a algoritmului.
    Asigură parametrii inițiali corecți pentru aplicarea funcției astar'.
-}

astar :: (ProblemState s a, Ord s) => Node s a -> Node s a
astar initialNode = astar' S.empty pqInitial
    where
        pqInitial = (PQ.insert initialNode ((nodeHeuristic initialNode) + fromIntegral (nodeDepth initialNode)) PQ.empty)

{-
    *** TODO ***
    Pornind de la un nod, reface parțial calea către nodul inițial, urmând legăturile
    către părinți.
    Întoarce o listă de perechi (acțiune, stare), care pornește de la starea următoare
    stării inițiale și se încheie la starea finală.
    ATENȚIE: Nodul inițial este singurul exclus!
-}

extractPath :: Node s a -> [(a, s)]
extractPath goalNode = reverse [(fromJust (nodeAction n), nodeState n) | n <- parents]
    where
        parents = takeWhile (\n -> nodeDepth n > 0) $ iterate (fromJust. nodeParent) goalNode