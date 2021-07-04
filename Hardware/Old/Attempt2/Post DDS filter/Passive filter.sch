<Qucs Schematic 0.0.20>
<Properties>
  <View=0,0,918,800,1,0,0>
  <Grid=10,10,1>
  <DataSet=Passive filter.dat>
  <DataDisplay=Passive filter.dpl>
  <OpenDisplay=1>
  <Script=Passive filter.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Drawn By:>
  <FrameText2=Date:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <Vac V1 1 190 370 18 -26 0 1 "1 V" 1 "1 GHz" 0 "0" 0 "0" 0>
  <GND * 5 190 410 0 0 0 0>
  <.DC DC1 1 50 40 0 37 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
  <L L2 1 580 300 -26 10 0 0 "4.7 uH" 1 "" 0>
  <L L3 1 710 300 -26 10 0 0 "4.7 uH" 1 "" 0>
  <L L4 1 850 300 -26 10 0 0 "3.3 uH" 1 "" 0>
  <C C3 1 780 360 17 -26 0 1 "47 pF" 1 "" 0 "neutral" 0>
  <C C2 1 640 360 17 -26 0 1 "56 pF" 1 "" 0 "neutral" 0>
  <GND * 5 640 410 0 0 0 0>
  <GND * 5 780 410 0 0 0 0>
  <GND * 5 490 410 0 0 0 0>
  <C C1 1 490 360 17 -26 0 1 "47 pF" 1 "" 0 "neutral" 0>
  <L L1 1 410 300 -26 10 0 0 "3.3 uH" 1 "" 0>
  <.AC AC1 1 290 60 0 37 0 0 "lin" 1 "1 MHz" 1 "50 MHz" 1 "1000" 1 "no" 0>
</Components>
<Wires>
  <190 400 190 410 "" 0 0 0 "">
  <640 390 640 410 "" 0 0 0 "">
  <780 390 780 410 "" 0 0 0 "">
  <610 300 640 300 "" 0 0 0 "">
  <640 300 680 300 "" 0 0 0 "">
  <640 300 640 330 "" 0 0 0 "">
  <780 300 780 330 "" 0 0 0 "">
  <740 300 780 300 "" 0 0 0 "">
  <780 300 820 300 "" 0 0 0 "">
  <490 390 490 410 "" 0 0 0 "">
  <490 300 550 300 "" 0 0 0 "">
  <490 300 490 330 "" 0 0 0 "">
  <440 300 490 300 "" 0 0 0 "">
  <190 300 190 340 "" 0 0 0 "">
  <190 300 380 300 "" 0 0 0 "">
  <880 300 880 300 "Uout" 910 270 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
