-- 向聴計算用 面子計算表構築プログラム

import System.Process

type NumsOfTile = (Int, Int, Int, Int, Int, Int, Int, Int, Int) -- 牌式にあたる型
type NumsOfMeld = (Bool, Int, Int) -- 対子フラグ、面子数、面子候補数

testVal :: NumsOfTile
testVal = (1, 0, 1, 2, 1, 0, 1, 0, 0)
testVal2 :: NumsOfTile
testVal2 = (3, 1, 1, 1, 0, 0, 0, 0, 0)
testVal3 :: NumsOfTile
testVal3 = (0, 1, 1, 3, 1, 1, 0, 0, 0)
testVal4 :: NumsOfTile
testVal4 = (1, 0, 1, 1, 1, 0, 0, 0, 0)
testVal5 :: NumsOfTile
testVal5 = (1, 0, 0, 3, 3, 3, 3, 0, 0)


(#) :: NumsOfTile -> Int -> Int -- NumsOfTileから値取り出し
(#) nums 1 = let (n,_,_,_,_,_,_,_,_) = nums in n
(#) nums 2 = let (_,n,_,_,_,_,_,_,_) = nums in n
(#) nums 3 = let (_,_,n,_,_,_,_,_,_) = nums in n
(#) nums 4 = let (_,_,_,n,_,_,_,_,_) = nums in n
(#) nums 5 = let (_,_,_,_,n,_,_,_,_) = nums in n
(#) nums 6 = let (_,_,_,_,_,n,_,_,_) = nums in n
(#) nums 7 = let (_,_,_,_,_,_,n,_,_) = nums in n
(#) nums 8 = let (_,_,_,_,_,_,_,n,_) = nums in n
(#) nums 9 = let (_,_,_,_,_,_,_,_,n) = nums in n
infixl 9 #

toList :: NumsOfTile -> [Int] -- NumsOfTileをリストに変換
toList nums = let (n1,n2,n3,n4,n5,n6,n7,n8,n9) = nums
              in [n1,n2,n3,n4,n5,n6,n7,n8,n9]

isEmpty :: NumsOfTile -> Bool -- 牌がない場合True
isEmpty (0,0,0,0,0,0,0,0,0) = True
isEmpty _                   = False

isTooMany :: NumsOfTile -> Bool -- 牌が多すぎる場合True
isTooMany nums = (foldl (+) 0 (toList nums)) > 14

isIsolated :: NumsOfTile -> Int -> Bool -- 浮き牌かどうか
isIsolated nums 1 = let (n,p,q,_,_,_,_,_,_) = nums
                    in (n == 1) && (p == 0) && (q == 0)
isIsolated nums 9 = let (_,_,_,_,_,_,q,p,n) = nums
                    in (n == 1) && (p == 0) && (q == 0)
isIsolated nums 2 = let (r,n,p,q,_,_,_,_,_) = nums
                    in (n == 1) && (p == 0) && (q == 0) && (r == 0)
isIsolated nums 8 = let (_,_,_,_,_,q,p,n,r) = nums
                    in (n == 1) && (p == 0) && (q == 0) && (r == 0)
isIsolated nums x = let n = nums # x
                        n1 = nums # (x - 1)
                        n2 = nums # (x - 2)
                        n3 = nums # (x + 1)
                        n4 = nums # (x + 2)
                    in (n == 1) && (n1 == 0) && (n2 == 0) && (n3 == 0) && (n4 == 0)

filterNums :: NumsOfTile -> (NumsOfTile -> Int -> Int) -> NumsOfTile
filterNums nums f = (f nums 1, f nums 2, f nums 3,
                     f nums 4, f nums 5, f nums 6,
                     f nums 7, f nums 8, f nums 9)

dropIsolates :: NumsOfTile -> NumsOfTile -- 浮き牌は除去しておく
dropIsolates nums = filterNums nums (\p q -> if isIsolated p q then 0 else p # q)


takeTriplet :: NumsOfTile -> NumsOfMeld -> Int -> (NumsOfTile, NumsOfMeld) -- 刻子を取る
takeTriplet nums mlds i = let (pairFlag, melds, semimelds) = mlds
                          in if nums # i >= 3
                                then (filterNums nums (\p q -> if q == i then p # q - 3 else p # q), (pairFlag, melds + 1, semimelds))
                                else (nums, mlds)
takeSequence :: NumsOfTile -> NumsOfMeld -> Int -> (NumsOfTile, NumsOfMeld) -- 順子を取る
takeSequence nums mlds i = let (pairFlag, melds, semimelds) = mlds
                           in if (nums # i >= 1) && (nums # (i + 1) >= 1) && (nums # (i + 2) >= 1)
                                 then (filterNums nums (\p q -> if (q >= i) && (q <= (i + 2)) then p # q - 1 else p # q), (pairFlag, melds + 1, semimelds))
                                 else (nums, mlds)
takePair :: NumsOfTile -> NumsOfMeld -> Int -> (NumsOfTile, NumsOfMeld) -- 対子を取る
takePair nums mlds i = let (pairFlag, melds, semimelds) = mlds
                       in if nums # i >= 2
                             then (filterNums nums (\p q -> if q == i then p # q - 2 else p # q), (True, melds, semimelds + 1))
                             else (nums, mlds)
takeStep1 :: NumsOfTile -> NumsOfMeld -> Int -> (NumsOfTile, NumsOfMeld) -- 両面と辺張を取る
takeStep1 nums mlds i = let (pairFlag, melds, semimelds) = mlds
                        in if (nums # i >= 1) && (nums # (i + 1) >= 1)
                              then (filterNums nums (\p q -> if (q >= i) && (q <= (i + 1)) then p # q - 1 else p # q), (pairFlag, melds, semimelds + 1))
                              else (nums, mlds)
takeStep2 :: NumsOfTile -> NumsOfMeld -> Int -> (NumsOfTile, NumsOfMeld) -- 嵌張を取る
takeStep2 nums mlds i = let (pairFlag, melds, semimelds) = mlds
                        in if (nums # i >= 1) && (nums # (i + 2) >= 1)
                              then (filterNums nums (\p q -> if (q == i) || (q == (i + 2)) then p # q - 1 else p # q), (pairFlag, melds, semimelds + 1))
                              else (nums, mlds)

maxMeld :: Int -> (NumsOfTile, NumsOfMeld) -> (NumsOfTile, NumsOfMeld) -> (NumsOfTile, NumsOfMeld) -- 最大のものを計算する
maxMeld mode (nums1, mlds1) (nums2, mlds2) = let (pairFlag1, melds1, semimelds1) = mlds1
                                                 (pairFlag2, melds2, semimelds2) = mlds2
                                             in if (melds1 * mode + semimelds1) < (melds2 * mode + semimelds2)
                                                   then (nums2, mlds2)
                                                   else if ((melds1 * mode + semimelds1) == (melds2 * mode + semimelds2)) && (not pairFlag1) && pairFlag2
                                                           then (nums2, mlds2)
                                                           else (nums1, mlds1)

enumerateMelds :: (NumsOfTile -> NumsOfMeld -> Int -> (NumsOfTile, NumsOfMeld)) -> [Int] -> NumsOfTile -> NumsOfMeld -> [(NumsOfTile, NumsOfMeld)] -- 面子列挙
enumerateMelds f l nums mlds = let (_, melds, semimelds) = mlds
                               in filter (\(_, (_, m, s)) -> (m /= melds) || (s /= semimelds)) [f (dropIsolates nums) mlds k | k <- l]

calculate :: Int -> NumsOfTile -> NumsOfMeld -> (NumsOfTile, NumsOfMeld) -- 計算（再帰処理）
calculate mode nums mlds = if isEmpty $ dropIsolates nums
                              then (nums, mlds)
                              else if n1 == 0
                                      then calculate mode (n2,n3,n4,n5,n6,n7,n8,n9,0) mlds
                                      else foldl (maxMeld mode) (nums, mlds) (
                                                  [calculate mode (fst p) (snd p) | p <- enumerateMelds takeTriplet [1..9] (dropIsolates nums) mlds] ++
                                                  [calculate mode (fst p) (snd p) | p <- enumerateMelds takeSequence [1..7] (dropIsolates nums) mlds] ++
                                                  [calculate mode (fst p) (snd p) | p <- enumerateMelds takePair [1..9] (dropIsolates nums) mlds] ++
                                                  [calculate mode (fst p) (snd p) | p <- enumerateMelds takeStep1 [1..8] (dropIsolates nums) mlds] ++
                                                  [calculate mode (fst p) (snd p) | p <- enumerateMelds takeStep2 [1..7] (dropIsolates nums) mlds] )
    where (n1,n2,n3,n4,n5,n6,n7,n8,n9) = nums

calcMentz :: Int -> NumsOfTile -> Maybe NumsOfMeld
calcMentz mode nums = if isTooMany nums then Nothing
                                        else Just (snd (calculate mode nums (False, 0, 0)))

idToMentzDat :: Int -> NumsOfTile -- 通し番号から変換
idToMentzDat i = ((i `mod`      5),
                  (i `mod`     25) `div`      5,
                  (i `mod`    125) `div`     25,
                  (i `mod`    625) `div`    125,
                  (i `mod`   3125) `div`    625,
                  (i `mod`  15625) `div`   3125,
                  (i `mod`  78125) `div`  15625,
                  (i `mod` 390625) `div`  78125,
                  (i             ) `div` 390625 )

meldToByte :: Maybe NumsOfMeld -> Int -- byteに直す
meldToByte Nothing = 255
meldToByte (Just (True, p, q)) = 128 + p * 16 + q
meldToByte (Just (False, p, q)) = p * 16 + q

putChrs :: [Int] -> IO () -- まとめて出力
putChrs chrCode = do system ("./putchr " ++ (unwords [show i | i <- chrCode]))
                     return ()

totalPatterns = 5*5*5*5*5*5*5*5*5 :: Int -- パターン合計
batch = 5*5*5 :: Int -- この件数だけまとめて出力

outputJob :: Int -> IO ()
outputJob j = putChrs [meldToByte $ (calcMentz n) $ idToMentzDat (i + j) | i <- [0..(batch - 1)], n <- [2, 10]]

main = sequence [outputJob k | k <- [0, batch .. (totalPatterns - 1)]]
