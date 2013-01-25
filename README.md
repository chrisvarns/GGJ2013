GGJ2013
=======

Commit rules.
1. Work on feature branches. i.e. branch from master with "git checkout -b <new_branch> and work there.
2. Before merging back to master, ensure you rebase your work back onto master (essentially incorporate any changes that have occured on master since your branch).
3. Any merge issues will occur at this point, so you can fix them before merging back to master with "git checkout master; git merge --no-ff branch"
