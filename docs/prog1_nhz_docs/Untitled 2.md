---
created: 2021-11-29T02:44:04 (UTC +01:00)
tags: []
source: https://medium.com/@brazmogu/physics-for-game-dev-a-platformer-physics-cheatsheet-f34b09064558
author: Bruno Guedes
---

# Physics for Game Dev — A Platformer Physics Cheatsheet | by Bruno Guedes | Medium

> ## Excerpt
> A quick primer on a few basic equations of platforming movement and a source for quick information on how to calculate a few key values.

---
[![Bruno Guedes](https://miro.medium.com/fit/c/56/56/1*Z6Q9xM3mCbIhqL_zOaYsjQ.jpeg)](https://medium.com/@brazmogu?source=post_page-----f34b09064558-----------------------------------)

Hello, everyone. You might know me from other such hit articles such as [my tutorial on making a Godot game](https://medium.com/@brazmogu/your-first-godot-game-a-live-tutorial-2a0423243f0e) and… that's about it so far. See, this is why I'm making this, because I've figured that, as far as sidegigs go, I can write tutorials and educational articles a lot better than I can make assets or art commissions, although I'd love to. One has to play to their strengths, I suppose.

So, this is another article on my way to figuring my own pace and style, and maybe even becoming kind of pro at this. I like the idea of teaching, and especially teaching game development. Nowadays is so much easier than it was when I wanted to learn about it, but still it's good to contribute to the effort. So, if you're here making valuable use of my knowledge… I'm very glad. Enjoy yourself!

All that said, let’s begin. Today’s lecture is going to be about… Physics!

Platformers are, essentially, all about jumping. Jumping is basically a parable, specifically one that curves downward, with vertical velocity starting up and gradually being pulled down again until the character hits the ground. In other words…

![](https://miro.medium.com/max/60/1*F8hRSZLTF8D3-Rg3GQosPg.png?q=20)

![](https://miro.medium.com/max/1400/1*F8hRSZLTF8D3-Rg3GQosPg.png)

All graphics cooked up in Wolfram Alpha

In generic terms, a downward parable can be defined by a second degree equation, as such:

**_f(x)_** _= ax² + bx + c_

With the only limitation being that the value of _a_ must be negative; if it’s positive, your curve will point upwards, and if it’s zero, then you have a straight line instead of a curve. Being more specific, even, all of these values actually have a specific meaning:

-   _f(x)_ is the vertical position, relative to the ground
-   _x_, a bit confusingly, is the time since the start of the jump
-   _a_ relates to gravity (it's negative, because it points downwards)
-   _b_ relates to velocity, or more specifically, the initial jump velocity (it's positive, because it points upwards)

So, if we're to rewrite this equation to seem more familiar, you can say that the jump is described as:

_y = -0.5gt² + v't_

From this one equation we can derive a lot of others, although those of you with some cursory knowledge of Basic Mechanics might argue it's the opposite¹. It doesn't matter much. Let's get to the math!

## Gravity

Ok, before we get to the math, let's discuss this parameter. The gravity in a platformer game pulls downward, much like it does in the real world². While the value of gravity will influence the movement, in reality we'll be working in a way that it won't. But it will.

Sorry, that got confusing. The role of gravity in the design of platforming movement is that the larger gravity is, the quicker the jump will be. That will have the effect of making the curve of the parable wider or narrower. A little bit ahead we'll see that we can determine the height of the jump independent of the gravity itself, so what will happen is that changing gravity while keeping the same jump velocity will make the jump shorter.

![](https://miro.medium.com/max/1400/1*tH43XXMukNwGhDvo4kotag.png)

Gravity varying through 0.5, 1 and 2, same jump speed

Meanwhile, if you maintain the jump height (by varying the jump speed accordingly), you'll end up with a jump that's more… "floaty" the smaller the gravity.

![](https://miro.medium.com/max/1400/1*gTE8PEFkL4cK4h_udWxqlA.png)

gravity varying through 0.5, 1 and 2, same jump height

As for how to maintain the same height for varying values of gravity, well…

## Jump Height (and Velocity)

Knowing how high your character can jump is a very important point of design in platforming games, for reasons that should be obvious. You can determine that by experimenting with gravity and jump speed, yes, but you can also use math to just figure out whatever your jump speed needs to be for your character to go exactly as high as you want them to.

For that, we'll take two equations, namely:

-   The equation of vertical position by time, i.e. _y = 0.5gt² + v't_
-   The equation of vertical speed by time, i.e. _v = v'+ gt_

Remembering, of course, that gravity is negative. So, we need to figure out at what time the character is at the highest point given whatever value of gravity, and that can be easily determined if you remember how vertical velocity works on a jump: the character starts moving up, velocity is gradually reduced by gravity and then becomes 0, until it starts pointing down and increasing again, and the character moves downward. _Ergo_, we need to figure out at what time _v = 0_. Using the second equation, we have that…

_v = v' + gt = 0_, thus _gt = -v'_ and finally³ _t = -v'/g_

Applying that enigmatic time signature into the first equation will give us the height of the character when velocity is 0, i.e., the highest it'll ever be. So, given a value _g_ of gravity, our character will jump _H_ units if the initial velocity _v'_ is…

_H = 0.5gt² + v't = 0.5g(-v'/g)² + v'(v'/g) = -0.5v'²/g + v'²/g = 0.5v'²/g_

Thus, _v'² = 2Hg_ which means that the initial velocity for the character to jump _H_ high is the square root of _2 \* H \* g._

## Jump Time (and Distance)

Next up, you’ll want to know how far the character goes when they jump. The trick to this is that what you want to determine here is the jump time, and to affect the jump time while keeping the same jump height is… gravity.

So, back to tweaking gravity, we'll want to change gravity in order to match a certain given air time. We start by picking that value of jump velocity that we found in the previous section and putting it right back in the equation that told us the time to reach the peak of the jump. That is…

_t = -v'/g => t² = -2Hg/g² = -2H/g_

So, given we want the time of the jump to be twice the time it takes to reach the topmost part of it _t'_, we have…

_t² = (2t')² = 4t'² = -2H/g => g = -2H/4t² = H/2t²_

And with this, you can determine the basic constant of platforming movement (gravity) given whatever height and duration of jump, creating a customizable physics component that can be easily tweaked without the need of complicated calculations just to find the right basic parameters.

So, to recap all we've been through, or in case you're just here to copy these basic equations to use in your game⁴:

Given a chosen height of jump _H_ and duration _t_, the parameters of your physics system should be:

**_gravity_** _= H/2t²_

**_jump speed_** _= sqrt(2Hg)_

This is not _all_ that goes into a platformer, mind you… but dealing with friction, terminal velocity and other concepts are significantly more trivial, and then I can write a new tutorial only to discuss those concepts. :V

Anyway, this has been an educational article on gamedev things. May you have learned something useful, leave your questions if you have any and let's hope to meet again, in the near future. o/
