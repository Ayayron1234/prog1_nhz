---
created: 2021-11-29T02:45:27 (UTC +01:00)
tags: []
source: https://2dengine.com/?p=platformers
author: 
---

# Platformer Physics Tutorial

> ## Excerpt
> Movement in platformer games can be reduced to a few equations.
Let's say that every object has a position and a velocity which are integrated each frame.

---
## Gravity

Movement in platformer games can be reduced to a few equations. Let's say that every object has a position and a velocity which are integrated each frame.

```x
velocity = velocity + gravity\*dt
position = position + velocity\*dt
```

These equations work pretty good if you're in a vacuum. In a vacuum, gravity can accelerate a falling object forever and we definitely don't want mushrooms falling at the speed of light! One solution is to clamp the velocity of moving objects to a reasonable threshold.

```x
local v = math.sqrt(velocity.x^2 + velocity.y^2)
if v > maxVelocity then
  local vs = maxVelocity/v
  velocity.x = velocity.x*vs
  velocity.y = velocity.y*vs
end
```
## Damping

Damping is sometimes referred to as "friction" which is strange since it's actually more similar to air resistance. I suppose for overhead games it looks like friction, but I digress. Anyways, one simple equation used for damping is:

velocity = velocity / (1 + damping\*dt)

Where "damping" coefficient is a value between 0 to infinity. The greater this coefficient the faster the objects will slow down. To be more specific, a value between 0 to 1 is considered "under-damping", 1 is considered "critical damping" and any value greater than 1 is "over-damping".

## Jumping

Implementing a good jumping mechanic can be difficult due to several factors. Typically, we want to let the player control the height of his jumps... after taking off. Although this is against the laws of physics, it makes the gameplay more controlled. So let's look at the trickery that makes platformer jumping look and feel "right".

We have to start with some assumptions. First, the player will be controlling his jumps using a single button. The longer he holds the button, the higher we want him to jump. This creates the illusion that pressing the button harder makes your character jump higher. A naive approach might be to continually apply an upward force while the player is holding the jump button. Unfortunately, this produces "floaty" jumps and won't feel right unless the character is sporting a jetpack.  
![](https://2dengine.com/images/tutorials/jumping0.png)![](https://2dengine.com/images/tutorials/jumping1.png)  
Left: jetpack with thrust applied for different periods of time  
Right: jumps with variable initial velocity

if pressedJumpKey then
  player.yv = initJumpVelocity
end

The physics of jumping is similar to that of launching a projectile. We set the initial velocity of the character and let gravity do its thing (ignoring the effects of air resistance). This brings us to the second assumption: regardless of how long the player holds the jump button he always takes off with the same vertical velocity. Knowing the character's maximum jump height we can deduce a number of other constants:

\-- what is the gravity that would allow jumping to a given height?
g = (2\*jumpHeight)/(timeToApex^2)

-- what is the initial jump velocity? 
initJumpVelocity = math.sqrt(2\*g\*jumpHeight)

-- how long does it take to reach the maximum height of a jump?
-- note: if "initJumpVelocity" is not a multiple of "g" the maximum height is reached between frames
timeToApex = initJumpVelocity/g

Thanks to the three equations above, you no longer have to set the gravity in your game to some magic value. Nor do you have to tweak and test the character's jump velocity by hand!

### Calculator

Enter at least two constants (white) and the calculator will fill in the blanks (gray). Air resistance is not taken into account.

Jump height

Gravity

Initial velocity

Time to apex

Here are a few example values, provided as a reference:  

High jump (Olympics)=2.45 m  
Standing jump (Guinness record)=1.48 m  
Michael Jordan (Basketball)=1.22 m  

Jupiter's gravity=24.79 m/s2  
Earth's gravity=9.807 m/s2  
Mars' gravity=3.711 m/s2  
Moon's gravity=1.62 m/s2  

Escape velocity=11186 m/s  
Speed of sound=343 m/s  
Terminal velocity=54 m/s  
Baseball pitch=47 m/s  
Soccer kick=36 m/s  

Reaction (visual stimulus)=0.190 s  
Reaction (auditory)=0.160 s  
Reaction (touch)=0.150 s \*  
Blinking=0.125 s  

\* For an average human being, the reaction to an auditory stimulus is faster than that compared to visual stimulus. Therefore, sound effects are essential to making your platformer game "feel right". Check your sound effects and make sure to **remove the tiny interval of silence** at the beginning of each file! Interestingly, humans react even quicker to touch. You may want to **consider the use of force feedback or vibration** in your games. Did you know that Android subtly uses haptic feedback whenever you press on the on-screen keyboard?

### Jump detection

One naive approach to handling jumping is to allow the player to jump _only_ when his character is on the ground. It feels better to trigger the jump even when the button was pressed right before landing. The following example accepts button presses up to 0.2 seconds before we actually hit the ground.

```py
elapsed = 0
wantsToJump = false
function love.update(dt)
  elapsed = elapsed + dt
  wantsToJump = false
  if isGrounded then
    if lastJump and elapsed - lastJump < 0.2 then
      wantsToJump = true
    end
  end
  ...
end
```
lastJump = nil
function love.keypressed(key, scancode, isrepeat)
  if key == "space" then
    lastJump = elapsed
  end
end

It is also recommended to allow the player to jump for a split second after falling off ledges. The example below allows a 0.1 second grace period for jumping.

elapsed = 0
lastGrounded = 0
function love.update(dt)
  elapsed = elapsed + dt
  ...
  if isGrounded then
    lastGrounded = elapsed
  end
  if wantsToJump and elapsed - lastGrounded < 0.1 then
    -- jump!
  end
end

Both of these improvements combined can make your game controls feel much more forgiving.

### Jump termination

In the event that the player releases the jump button quickly, we have to fudge the laws of physics a little bit. The idea is to adjust the player's velocity mid-air so that he doesn't get up to his maximum jump height. Let's call this "jump termination".

### First technique: velocity reset

The simplest solution is to set the player's vertical velocity to zero as soon as he releases the jump button:

if releasedJumpKey then
  -- is the player ascending?
  if player.yv > 0 then
    player.yv = 0
  end
end

This approach is fine and produces sharp jumps like in Super Meat Boy. Unfortunately, it looks a little awkward for small jumps as the player takes off at a high velocity and immediately drops.  
![](https://2dengine.com/images/tutorials/jumping2.png)  
Jumps with the same initial velocity, modified by setting the vertical velocity to zero.  
Note how sharp the trajectory is for smaller jumps

### Second technique: velocity clamp

An alternative approach is to introduce another constant: "minimum jump height". Every time the character jumps, he is guaranteed to reach at least this minimum allowed height. As a consequence, the player can terminate his jumps only for a given amount of time ("termTime") before reaching the apex of his jump. When the player releases the jump button, instead of resetting the vertical velocity to zero, we clamp it to a predefined termination velocity ("termVelocity").

if releasedJumpKey then
  -- is the player ascending fast enough to allow jump termination?
  if player.yv > termVelocity then
    player.yv = termVelocity
  end
end

This approach make jumps feel like they have momentum. When the player releases the button, he continues to ascend for a little bit before gravity kicks in. The result is pretty good as long as the minimum allowed jump height is not too low. This is how jump termination would look when the minimum jump height is 1/5:  
![](https://2dengine.com/images/tutorials/jumping3.png)  
Jumps with the same initial velocity, modified by clamping the vertical velocity.  
Note that the trajectory is still not perfectly smooth for medium height jumps  
We can deduce the termination velocity ("termVelocity") and time ("termTime") as follows:

\-- what is the velocity required to terminate a jump?
-- note: only works when "g" is negative
termVelocity = math.sqrt(initJumpVelocity^2 + 2\*g\*(jumpHeight - minJumpHeight))

-- how much time is available until a jump can no longer be terminated?
-- note: "minJumpHeight" must be greater than 0
termTime = timeToApex - (2\*(jumpHeight - minJumpHeight)/(initJumpVelocity + termVelocity))

### Third technique: gravity scale

The third technique I would like to discuss comes from the Box2D physics library by Erin Catto. Basically, we introduce another variable called "gravity scale". Each object in the game could have a "gravity scale" property (it's default value being 1). Setting the "gravity scale" of an object to -1 would invert its gravity.

obj.velocity = obj.velocity + obj.gravityScale\*gravity\*dt
obj.position = obj.position + obj.velocity\*dt

Gravity scale is pretty useful if you want to have your character walk on the ceiling. In addition, when we set the gravity scale to 2, our character becomes twice as heavy! Let's see how this could be used to affect jumping. We modify the "gravity scale" once, when the player releases the jump button while in the air. The "gravity scale" is reset back to 1 immediately upon landing.

if isGrounded then
  player.gravityScale = 1
elseif releasedJumpKey then
  player.gravityScale = 2
end

The results don't look too bad for medium and high jumps. Note that smaller jump are steep and short in duration.  
![](https://2dengine.com/images/tutorials/jumping4.png)  
Jumps affected by modifying the gravity scale

## References

-   [5 Reasons Your Indie Platformer Game Sucks!](https://www.youtube.com/watch?v=vFsJIrm2btU) by Jonas Tyroller
-   [Platformer physics 101 and the 3 fundamental equations of platformers](http://error454.com/2013/10/23/platformer-physics-101-and-the-3-fundamental-equations-of-platformers/) by Zachary Burke
-   [Controlling jump height](https://michagamedev.wordpress.com/2014/05/25/controlling-jump-height/) by Micha
-   [Box2D](http://box2d.org/) by Erin Catto
