PATH=/bin:/usr/bin:/usr/local/bin
export PATH

rsync -avz minnie.tuhs.org::UA_Root .
rsync -avz minnie.tuhs.org::UA_Applications Applications
rsync -avz minnie.tuhs.org::UA_Distributions Distributions
rsync -avz minnie.tuhs.org::UA_Documentation Documentation
rsync -avz minnie.tuhs.org::UA_Tools Tools
