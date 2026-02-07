Thanks for sharing your results and the excerpt around `MPI_Reduce`. To answer your question about tolerance:

**Step 1: Tolerance Check**
- What is your tolerance threshold for π (e.g., absolute error < 1e-4)?
- Are the π values for p=1 and p=4 within this threshold compared to the true value (3.14159265...)?

**Step 2: If Not Within Tolerance**
- If the values are not within tolerance, possible causes include:
  - Incorrect sample splitting (each rank must get a unique subset).
  - RNG seeding issues (each rank must use a unique, reproducible seed).
  - Incorrect reduction (e.g., not summing all local counts).

**Step 3: Next Test**
- Print per-rank seeds and local counts before reduction.  
  *Why?* This verifies that each rank is using a unique seed and computing its own portion correctly.
- Confirm that the sum of local counts equals the total number of points inside the circle.

**Action:**  
Please share:
1. Your tolerance threshold.
2. The π values for p=1 and p=4.
3. The output of per-rank seeds and local counts.

Once you provide these, I’ll help you interpret and suggest the next minimal fix or test.