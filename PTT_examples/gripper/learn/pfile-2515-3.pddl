


(define (problem gripper-2-5-15)
(:domain gripper-strips)
(:objects robot1 robot2 - robot
rgripper1 lgripper1 rgripper2 lgripper2 - gripper
room1 room2 room3 room4 room5 - room
ball1 ball2 ball3 ball4 ball5 ball6 ball7 ball8 ball9 ball10 ball11 ball12 ball13 ball14 ball15 - object)
(:init
(at-robby robot1 room2)
(free robot1 rgripper1)
(free robot1 lgripper1)
(at-robby robot2 room5)
(free robot2 rgripper2)
(free robot2 lgripper2)
(at ball1 room5)
(at ball2 room4)
(at ball3 room4)
(at ball4 room5)
(at ball5 room4)
(at ball6 room1)
(at ball7 room3)
(at ball8 room1)
(at ball9 room2)
(at ball10 room5)
(at ball11 room4)
(at ball12 room3)
(at ball13 room2)
(at ball14 room5)
(at ball15 room3)
)
(:goal
(and
(at ball1 room2)
(at ball2 room4)
(at ball3 room2)
(at ball4 room2)
(at ball5 room4)
(at ball6 room5)
(at ball7 room3)
(at ball8 room3)
(at ball9 room3)
(at ball10 room2)
(at ball11 room1)
(at ball12 room1)
(at ball13 room3)
(at ball14 room5)
(at ball15 room2)
)
)
)


