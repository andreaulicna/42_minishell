## Git
- Switch to a branch: ```git checkout [my-branch-name]```
- Create and switch to a branch: ```git checkout -b [my-branch-name]```

## Valgrind
```valgrind -s --leak-check=full --show-reachable=yes --error-limit=no --suppressions=minishell.supp ./minishell;```