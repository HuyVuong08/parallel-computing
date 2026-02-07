To confirm your Slurm script enforces one task per node, I need to see the relevant lines—especially `#SBATCH --nodes`, `#SBATCH --ntasks`, and `#SBATCH --ntasks-per-node`. Please paste those lines here.

**What to check:**
- For one task per node:  
  `#SBATCH --ntasks-per-node=1`  
  and  
  `#SBATCH --ntasks=<number of nodes>`  
  or  
  `#SBATCH --nodes=<number of nodes>`

**For your trial times and averages:**  
Please paste the table or CSV with your 10 trial times and averages for each node count. I’ll check for consistency, outliers, and scaling issues.

**Action:**  
1. Paste the relevant Slurm script lines.
2. Paste your timing results table.

Once you do, I’ll verify your setup and analyze the timings for red flags.