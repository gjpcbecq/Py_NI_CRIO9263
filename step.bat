echo off
echo ===========================================================================
echo ___________________________________________________________________________
echo    Ch0
python step.py 0 100 0.1 &
echo ___________________________________________________________________________
echo ___________________________________________________________________________
echo    Ch1
python step.py 1 100 0.1 &
echo ___________________________________________________________________________
echo ___________________________________________________________________________
echo    Ch2
python step.py 2 100 0.1 &
echo ___________________________________________________________________________
echo ___________________________________________________________________________
echo    Ch3
python step.py 3 100 0.1 &
echo ___________________________________________________________________________
echo ___________________________________________________________________________
echo    All Chan
python stepAll.py 100 0.1 &
echo ___________________________________________________________________________
echo ===========================================================================


