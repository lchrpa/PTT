


(define (problem gripper-20-20-180)
(:domain gripper-strips)
(:objects robot1 robot2 robot3 robot4 robot5 robot6 robot7 robot8 robot9 robot10 robot11 robot12 robot13 robot14 robot15 robot16 robot17 robot18 robot19 robot20 - robot
rgripper1 lgripper1 rgripper2 lgripper2 rgripper3 lgripper3 rgripper4 lgripper4 rgripper5 lgripper5 rgripper6 lgripper6 rgripper7 lgripper7 rgripper8 lgripper8 rgripper9 lgripper9 rgripper10 lgripper10 rgripper11 lgripper11 rgripper12 lgripper12 rgripper13 lgripper13 rgripper14 lgripper14 rgripper15 lgripper15 rgripper16 lgripper16 rgripper17 lgripper17 rgripper18 lgripper18 rgripper19 lgripper19 rgripper20 lgripper20 - gripper
room1 room2 room3 room4 room5 room6 room7 room8 room9 room10 room11 room12 room13 room14 room15 room16 room17 room18 room19 room20 - room
ball1 ball2 ball3 ball4 ball5 ball6 ball7 ball8 ball9 ball10 ball11 ball12 ball13 ball14 ball15 ball16 ball17 ball18 ball19 ball20 ball21 ball22 ball23 ball24 ball25 ball26 ball27 ball28 ball29 ball30 ball31 ball32 ball33 ball34 ball35 ball36 ball37 ball38 ball39 ball40 ball41 ball42 ball43 ball44 ball45 ball46 ball47 ball48 ball49 ball50 ball51 ball52 ball53 ball54 ball55 ball56 ball57 ball58 ball59 ball60 ball61 ball62 ball63 ball64 ball65 ball66 ball67 ball68 ball69 ball70 ball71 ball72 ball73 ball74 ball75 ball76 ball77 ball78 ball79 ball80 ball81 ball82 ball83 ball84 ball85 ball86 ball87 ball88 ball89 ball90 ball91 ball92 ball93 ball94 ball95 ball96 ball97 ball98 ball99 ball100 ball101 ball102 ball103 ball104 ball105 ball106 ball107 ball108 ball109 ball110 ball111 ball112 ball113 ball114 ball115 ball116 ball117 ball118 ball119 ball120 ball121 ball122 ball123 ball124 ball125 ball126 ball127 ball128 ball129 ball130 ball131 ball132 ball133 ball134 ball135 ball136 ball137 ball138 ball139 ball140 ball141 ball142 ball143 ball144 ball145 ball146 ball147 ball148 ball149 ball150 ball151 ball152 ball153 ball154 ball155 ball156 ball157 ball158 ball159 ball160 ball161 ball162 ball163 ball164 ball165 ball166 ball167 ball168 ball169 ball170 ball171 ball172 ball173 ball174 ball175 ball176 ball177 ball178 ball179 ball180 - object)
(:init
(at-robby robot1 room1)
(free robot1 rgripper1)
(free robot1 lgripper1)
(at-robby robot2 room14)
(free robot2 rgripper2)
(free robot2 lgripper2)
(at-robby robot3 room11)
(free robot3 rgripper3)
(free robot3 lgripper3)
(at-robby robot4 room14)
(free robot4 rgripper4)
(free robot4 lgripper4)
(at-robby robot5 room11)
(free robot5 rgripper5)
(free robot5 lgripper5)
(at-robby robot6 room15)
(free robot6 rgripper6)
(free robot6 lgripper6)
(at-robby robot7 room12)
(free robot7 rgripper7)
(free robot7 lgripper7)
(at-robby robot8 room2)
(free robot8 rgripper8)
(free robot8 lgripper8)
(at-robby robot9 room6)
(free robot9 rgripper9)
(free robot9 lgripper9)
(at-robby robot10 room11)
(free robot10 rgripper10)
(free robot10 lgripper10)
(at-robby robot11 room8)
(free robot11 rgripper11)
(free robot11 lgripper11)
(at-robby robot12 room6)
(free robot12 rgripper12)
(free robot12 lgripper12)
(at-robby robot13 room20)
(free robot13 rgripper13)
(free robot13 lgripper13)
(at-robby robot14 room18)
(free robot14 rgripper14)
(free robot14 lgripper14)
(at-robby robot15 room9)
(free robot15 rgripper15)
(free robot15 lgripper15)
(at-robby robot16 room20)
(free robot16 rgripper16)
(free robot16 lgripper16)
(at-robby robot17 room3)
(free robot17 rgripper17)
(free robot17 lgripper17)
(at-robby robot18 room2)
(free robot18 rgripper18)
(free robot18 lgripper18)
(at-robby robot19 room15)
(free robot19 rgripper19)
(free robot19 lgripper19)
(at-robby robot20 room5)
(free robot20 rgripper20)
(free robot20 lgripper20)
(at ball1 room9)
(at ball2 room17)
(at ball3 room4)
(at ball4 room19)
(at ball5 room3)
(at ball6 room18)
(at ball7 room2)
(at ball8 room11)
(at ball9 room4)
(at ball10 room15)
(at ball11 room12)
(at ball12 room4)
(at ball13 room9)
(at ball14 room3)
(at ball15 room18)
(at ball16 room19)
(at ball17 room17)
(at ball18 room9)
(at ball19 room1)
(at ball20 room2)
(at ball21 room19)
(at ball22 room8)
(at ball23 room7)
(at ball24 room19)
(at ball25 room5)
(at ball26 room16)
(at ball27 room19)
(at ball28 room8)
(at ball29 room18)
(at ball30 room13)
(at ball31 room12)
(at ball32 room6)
(at ball33 room9)
(at ball34 room16)
(at ball35 room4)
(at ball36 room12)
(at ball37 room13)
(at ball38 room6)
(at ball39 room3)
(at ball40 room16)
(at ball41 room20)
(at ball42 room14)
(at ball43 room20)
(at ball44 room9)
(at ball45 room16)
(at ball46 room17)
(at ball47 room7)
(at ball48 room13)
(at ball49 room6)
(at ball50 room8)
(at ball51 room14)
(at ball52 room5)
(at ball53 room15)
(at ball54 room1)
(at ball55 room3)
(at ball56 room20)
(at ball57 room16)
(at ball58 room2)
(at ball59 room7)
(at ball60 room13)
(at ball61 room14)
(at ball62 room19)
(at ball63 room18)
(at ball64 room3)
(at ball65 room14)
(at ball66 room2)
(at ball67 room15)
(at ball68 room6)
(at ball69 room7)
(at ball70 room17)
(at ball71 room2)
(at ball72 room7)
(at ball73 room11)
(at ball74 room2)
(at ball75 room15)
(at ball76 room6)
(at ball77 room18)
(at ball78 room2)
(at ball79 room18)
(at ball80 room4)
(at ball81 room9)
(at ball82 room12)
(at ball83 room8)
(at ball84 room4)
(at ball85 room12)
(at ball86 room11)
(at ball87 room4)
(at ball88 room8)
(at ball89 room12)
(at ball90 room11)
(at ball91 room20)
(at ball92 room5)
(at ball93 room9)
(at ball94 room18)
(at ball95 room8)
(at ball96 room2)
(at ball97 room19)
(at ball98 room2)
(at ball99 room8)
(at ball100 room6)
(at ball101 room18)
(at ball102 room10)
(at ball103 room12)
(at ball104 room8)
(at ball105 room11)
(at ball106 room7)
(at ball107 room14)
(at ball108 room9)
(at ball109 room8)
(at ball110 room12)
(at ball111 room12)
(at ball112 room17)
(at ball113 room4)
(at ball114 room19)
(at ball115 room1)
(at ball116 room16)
(at ball117 room9)
(at ball118 room4)
(at ball119 room3)
(at ball120 room1)
(at ball121 room14)
(at ball122 room3)
(at ball123 room6)
(at ball124 room3)
(at ball125 room20)
(at ball126 room13)
(at ball127 room5)
(at ball128 room19)
(at ball129 room14)
(at ball130 room12)
(at ball131 room4)
(at ball132 room12)
(at ball133 room1)
(at ball134 room15)
(at ball135 room20)
(at ball136 room12)
(at ball137 room1)
(at ball138 room14)
(at ball139 room20)
(at ball140 room9)
(at ball141 room5)
(at ball142 room11)
(at ball143 room5)
(at ball144 room9)
(at ball145 room10)
(at ball146 room5)
(at ball147 room4)
(at ball148 room19)
(at ball149 room9)
(at ball150 room7)
(at ball151 room19)
(at ball152 room2)
(at ball153 room9)
(at ball154 room4)
(at ball155 room4)
(at ball156 room9)
(at ball157 room16)
(at ball158 room8)
(at ball159 room7)
(at ball160 room10)
(at ball161 room20)
(at ball162 room10)
(at ball163 room2)
(at ball164 room1)
(at ball165 room5)
(at ball166 room1)
(at ball167 room12)
(at ball168 room6)
(at ball169 room15)
(at ball170 room11)
(at ball171 room14)
(at ball172 room19)
(at ball173 room2)
(at ball174 room19)
(at ball175 room8)
(at ball176 room11)
(at ball177 room3)
(at ball178 room11)
(at ball179 room9)
(at ball180 room11)
)
(:goal
(and
(at ball1 room18)
(at ball2 room8)
(at ball3 room13)
(at ball4 room6)
(at ball5 room11)
(at ball6 room17)
(at ball7 room15)
(at ball8 room7)
(at ball9 room5)
(at ball10 room1)
(at ball11 room17)
(at ball12 room4)
(at ball13 room11)
(at ball14 room18)
(at ball15 room4)
(at ball16 room15)
(at ball17 room19)
(at ball18 room15)
(at ball19 room20)
(at ball20 room13)
(at ball21 room6)
(at ball22 room14)
(at ball23 room12)
(at ball24 room7)
(at ball25 room12)
(at ball26 room20)
(at ball27 room17)
(at ball28 room15)
(at ball29 room11)
(at ball30 room6)
(at ball31 room6)
(at ball32 room8)
(at ball33 room13)
(at ball34 room18)
(at ball35 room14)
(at ball36 room4)
(at ball37 room15)
(at ball38 room8)
(at ball39 room11)
(at ball40 room19)
(at ball41 room9)
(at ball42 room8)
(at ball43 room2)
(at ball44 room19)
(at ball45 room5)
(at ball46 room6)
(at ball47 room13)
(at ball48 room4)
(at ball49 room1)
(at ball50 room13)
(at ball51 room17)
(at ball52 room6)
(at ball53 room6)
(at ball54 room9)
(at ball55 room13)
(at ball56 room18)
(at ball57 room8)
(at ball58 room10)
(at ball59 room12)
(at ball60 room18)
(at ball61 room15)
(at ball62 room18)
(at ball63 room5)
(at ball64 room8)
(at ball65 room16)
(at ball66 room18)
(at ball67 room12)
(at ball68 room10)
(at ball69 room6)
(at ball70 room2)
(at ball71 room8)
(at ball72 room14)
(at ball73 room9)
(at ball74 room10)
(at ball75 room12)
(at ball76 room14)
(at ball77 room16)
(at ball78 room5)
(at ball79 room18)
(at ball80 room16)
(at ball81 room18)
(at ball82 room14)
(at ball83 room2)
(at ball84 room4)
(at ball85 room3)
(at ball86 room14)
(at ball87 room1)
(at ball88 room10)
(at ball89 room3)
(at ball90 room13)
(at ball91 room7)
(at ball92 room18)
(at ball93 room11)
(at ball94 room12)
(at ball95 room5)
(at ball96 room6)
(at ball97 room10)
(at ball98 room17)
(at ball99 room15)
(at ball100 room16)
(at ball101 room18)
(at ball102 room3)
(at ball103 room9)
(at ball104 room7)
(at ball105 room12)
(at ball106 room1)
(at ball107 room1)
(at ball108 room8)
(at ball109 room6)
(at ball110 room19)
(at ball111 room3)
(at ball112 room3)
(at ball113 room13)
(at ball114 room5)
(at ball115 room7)
(at ball116 room15)
(at ball117 room19)
(at ball118 room8)
(at ball119 room4)
(at ball120 room1)
(at ball121 room1)
(at ball122 room11)
(at ball123 room19)
(at ball124 room11)
(at ball125 room3)
(at ball126 room3)
(at ball127 room16)
(at ball128 room13)
(at ball129 room20)
(at ball130 room11)
(at ball131 room8)
(at ball132 room18)
(at ball133 room13)
(at ball134 room17)
(at ball135 room4)
(at ball136 room5)
(at ball137 room17)
(at ball138 room5)
(at ball139 room12)
(at ball140 room3)
(at ball141 room3)
(at ball142 room15)
(at ball143 room5)
(at ball144 room15)
(at ball145 room19)
(at ball146 room12)
(at ball147 room10)
(at ball148 room17)
(at ball149 room19)
(at ball150 room14)
(at ball151 room18)
(at ball152 room19)
(at ball153 room5)
(at ball154 room16)
(at ball155 room9)
(at ball156 room7)
(at ball157 room19)
(at ball158 room4)
(at ball159 room19)
(at ball160 room18)
(at ball161 room14)
(at ball162 room7)
(at ball163 room15)
(at ball164 room7)
(at ball165 room3)
(at ball166 room19)
(at ball167 room11)
(at ball168 room20)
(at ball169 room4)
(at ball170 room2)
(at ball171 room2)
(at ball172 room6)
(at ball173 room17)
(at ball174 room7)
(at ball175 room1)
(at ball176 room16)
(at ball177 room18)
(at ball178 room10)
(at ball179 room12)
(at ball180 room16)
)
)
)


