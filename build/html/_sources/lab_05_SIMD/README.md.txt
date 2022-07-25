# 05 - Incremental computation of moments
____
## 1. Introduction

In this exercise we want to begin getting a working knowledge of NEON SIMD inmstructions. We do this using a simple data reduction example, an online-calculation of moments of a statistical distribution.   

No points will be awarded for completion of this laboratory exercise.
You can do this exercise in groups.
One laboratory period (1.5 hrs.) is considered adequate for this exercise

____
## 2. Learning Aims
1. The format and usage of ARM-NEON intrinsic instructions
2. Online statistical calculations  

____
## 3. Task 1: Compute the Third Moment 
**Recommended ET & IT**

### Moments
Central moments are widely used in descriptive statistics. Standard approaches for computing them require two passes over the data. In applications were were data is streamed incremental results are needed after each new value is observed.

The first moment (mean) is given by 

![First Moment of a Distribution](./img/FirstMoment.PNG)

The second moment (variance) of a distrubtion is given by

![Second Moment of a Distribution](./img/SecondMoment.PNG)

The third moment (skew) is given by 

![Third Moment of a Distribution](./img/ThirdMoment.PNG)

The nth moment (kurtosis ... ) of a distrubtion is given by

![Fourth Moment of a Distribution](./img/NthMoment.PNG)

### Task
The example generates samples with a given Gaussian distribution. For each sample the new moments are calculated normally (`compute_moments_neon`) and with SIMD (`compute_moments_neon`). The `compute_moments_neon` function is incomplete as the third moment is not yet implemented.

Examine the given source code and answer the following questions:

1. How many dataset are calculated in parallel when using SIMD? 
2. Is the number of datasets given when using SIMD (check the ARM Neon reference)?
3. The example can be build using `make'
4. Currently only the first two moments are calculated

5. Compare the calculation of the first two moments to the implementation in `compute_moments` and identify the used intrinsics.
6. Write the calculation of the third moment in the `computes_moments_neon` function. You can use a `compute_moments` function as a blueprint. Don not forget to check if you have to add load and store instruction at the start or the end of the function.
7. Compare your results of the two implementations. Are there differences?
8. Is there any run-time improvement to be expected by using openMP?

___
## 4. Evaluation

This laboratory is not evaluated Nevertheless I **welcome** in-depth discussions on the contents of the lab and your solutions **during** the implementation process.  

___
## 5. References

https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/intrinsics

___
## 6. Version

| Version | Date | By | Comments | Class Level | Module |
| :-- | :-- | :-- | :-- | :-- | :-- |
| V1.1 | 04.2022 | donn | Added some sections | 6S | MPC |
| V1.0 | 02.2021 | donn | First version | 6S | MPC |
