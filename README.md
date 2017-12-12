PTT (Planning Task Transformer) 

author: Lukas Chrpa

=============================================
BUILDING PTT
=============================================

Requires cmake.

PTT is build by:

./build.sh

=============================================
EXECUTING PTT
=============================================

PTT is invoked as follows:

./ptt <config_file>

Alternatively, PTT can be invoked to generates macros online, i.e., the OMA method (see the paper: "On the Online Generation of Effective Macro-operators", IJCAI 2015):

./ptt -g <domain_file> <problem_file>


The outputs of PTT are reformulated domain and problem files (having the ".ref" suffix), and the "knowledge.kf" file with information about macros and entanglements that have been learnt.


=============================================
CONFIGURATION FILE
=============================================

;the domain file
domain=<domain_file>

;the training problem file with the corresponding plan file (for learning)
training=<problem_file> <plan_file>

;the testing problem file (to be reformulated)
problem=<problem_file>


;learning entanglements (see papers: "On Exploiting Structures of Classical Planning Problems: Generalizing Entanglements.", ECAI 2012, and "Revisiting Inner Entanglements in Classical Planning.", SCAI 2013)
flawratio=X (sets the flaw ratio N=[0.0,1.0], in the experiments in the papers, 0.1 has been used)
entanglements=init goal prec succ (types of entanglements to be extracted, i.e., entanglements by init,goal,preceding and succeeding respectively. Any subset can be used, i.e., entanglements=init goal  will learn outer entanglements)

;learning macros as in the paper: "Generation of macro-operators via investigation of action dependencies in plans", Knowledge Engineering Review 2010
macros=B C D (B=[0.0,1.0] stands for a lower bound for eliminating macro candidates that do not frequently appear successively - 1.0 means that the given operators are always successive, C=[0.0,1.0] stands for a lower bound for eliminating macro candidates that do not occur frequently in training plans - 0.0 means that the candidate never occurs in the training plans, D stands for an upper bound of macro arguments)

;the MUM method (see the paper: "MUM: A Technique for Maximising the Utility of Macro-operators by Constrained Generation and Use", ICAPS 2014)
flawratio=X (sets the flaw ratio N=[0.0,1.0], in the experiments in the papers, 0.1 has been used)
entanglements=init goal
macroentanglements=outer

;learning macros from inner entanglements (see the paper: "Generating Macro-Operators by Exploiting Inner Entanglements", SARA 2013)
flawratio=X (sets the flaw ratio N=[0.0,1.0], in the experiments in the papers, 0.1 has been used)
entanglements=prec succ
macroentanglements=inner




 
