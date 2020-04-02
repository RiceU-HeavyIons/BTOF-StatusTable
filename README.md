# BTOF-StatusTable
this script is to generate the bTOF status table 
to run the code:
1. you need to copy the codes to your own working directory: git clone https://github.com/RiceU-HeavyIons/BTOF-StatusTable.git
2. input file: inputfiles/allhists_run18isobar_forT0_pass7.root are generated from StBTofQAMaker 
3. to run the code: root -b -q -l genTofStatusResTable.C, it will read in the input root file, and generate the bad Trays, Modules, Cells into a table file named as "badTrayModuleCell_Run18.dat"

If you look into the code genTofStatusResTable.C, you will find it read in the 2D histograms of "T_{Measured}-T_{Exprected}" vs Trays or Modules or Cells. 
we are loop over these three histograms one by as the codes tells to tag find out which trays, modules and cells are bad. 
please note that, after the loop over trays, when we are looping over modules, we should not print those modules belonging to the already idenfified bad trays. Then when we searching for the bad cells, we should not print out those cells belong to the previous bad trays and modules. 

4. after the badTrayModuleCell_Run18.dat is generated, you need to put the timestamp (for example, 2018-01-15 00:00:02) on top of the this table as what in tofStatus_Run18.txt.

5. if everything works well, then you can copy this tofStatus_Run18.txt to rcf and upload it to the STAR DB. root4star -b -q -l tofStatus.C
The codes can be found at:
/star/u/zye20/allScripts/serviceWork/TOF_VPD_Calibration/calib/btof_ntuple_maker/params/Run18/Isobar_Zaochen/upload2DB/macros 

