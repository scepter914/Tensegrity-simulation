
## Tensegrity Simulation

This is simulation of tensegrity via openFrameworks.

## OS and library version

- Ubuntu16.04LTS
- openFrameworks v0.9.8
    - I don’t use physical engine like ofBox2d.

## video

- /video


## Methods

### Sequence

1. Initialize position of particles and parameters
2. Loop
    1. Initialize force of each particle 
    2. Update force of each particle from spring
    3. Update position of each particle
    4. Satisfy distance constraints of each strut
    5. Update collision of ground

### Each Particles

- f = Mg

### Spring

- $f=K_s(l-l_0)d+K_d(v_1・d-v_0・d)d$
- $K_s$ : spring constant 
- $l$ : current length 
- $l_0$ : rest length 
- $d$ : unit vector toward the other particle
- $K_d$ : damper constant
- $v_0$,$v_1$ : velocity of particles

### Update particles

- Verlet Integration
- $x_{n+1}$ ←  $x_n$ + ($x_n$-$x_{n - 1}$) + $Δt^2$ $F_n$ / $m$

### Strut

- $x_0$ ←  $(x_0 + x_1)/2.0$ - $(l_0/2.0)$ $d$ 
    - d : the unit vector toward the other particle
    - $l_0$ is the rest length.
- This update does not move the center of gravity of the strut.

### Update collision of graound

If the z value of a particle is under the particle radius, the particle
have to jump. I use the Position-based method for collision against
the ground.

- $ z_{n+1} = r $
- $ z_n ← r + z_{n+1} - z_n $ 

z is the z position of a particle. r is the radius of particles. 

For x, y, I add friction for collision.

- $ x_n = x_n + (x_{n+1} - x_n)(1.0 - u) $
- $ y_n = y_n + (y_{n+1} - y_n)(1.0 - u) $

### Parameters

- item Common value
    - $M$ = 0.20
    - $¥Delta t$ = 0.010 
    - $r$ = 5.0
    - $u$ = 0.80

- video1
    - $¥bm{x}$ = (-400.0, 200.0, 250.0)
    - $¥bm{v}$ = (-0.70, 0.80, 0.50)
    - $ K_s$ = 20.0 
    - $ K_b$ = 5.0 
    - $ g $ = -9.8

- Video2
    - $¥bm{x}$ = (-500.0, 800.0, 250.0)
    - $¥bm{v}$ = (-0.30, 1.00, 0.40) 
    - $ K_s$ = 20.0 
    - $ K_b$ = 5.0
    - $ g $ = -9.8

- Video3
    - $¥bm{x}$ = (-500.0, 800.0, 250.0)
    - $¥bm{v}$ = (-0.30, 1.00, 0.40) 
    - $ K_s$ = 20.0
    - $ K_b$ = 5.0 
    - $ g $ = -0.98

- Video4
    - $¥bm{x}$ = (-500.0, 800.0, 250.0)
    - $¥bm{v}$ = (-0.30, 1.00, 0.40)
    - $ K_s$ = 1.1 
    - $ K_b$ = 0.6 
    - $ g $ = -0.98 

### make movie

- run
    - make
    - make run
- make movie
    - cd bin/data
    - ffmpeg -r 200 -i image %05d.png -vcodec libx264 -pix fmt yuv420p -r 200 out.mp4

