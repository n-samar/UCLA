//
// xocc(TM)
// rundef.js: a xocc-generated Runs Script for WSH 5.1/5.6
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
//

echo "This script was generated under a different operating system."
echo "Please update the PATH variable below, before executing this script"
exit

var WshShell = new ActiveXObject( "WScript.Shell" );
var ProcEnv = WshShell.Environment( "Process" );
var PathVal = ProcEnv("PATH");
if ( PathVal.length == 0 ) {
  PathVal = "/opt/Xilinx/SDK/2018.2.op2258646/bin;/opt/Xilinx/SDx/2018.2.op2258646/bin;/opt/Xilinx/Vivado/2018.2.op2258646/bin;";
} else {
  PathVal = "/opt/Xilinx/SDK/2018.2.op2258646/bin;/opt/Xilinx/SDx/2018.2.op2258646/bin;/opt/Xilinx/Vivado/2018.2.op2258646/bin;" + PathVal;
}

ProcEnv("PATH") = PathVal;

var RDScrFP = WScript.ScriptFullName;
var RDScrN = WScript.ScriptName;
var RDScrDir = RDScrFP.substr( 0, RDScrFP.length - RDScrN.length - 1 );
var ISEJScriptLib = RDScrDir + "/ISEWrap.js";
eval( EAInclude(ISEJScriptLib) );


ISEStep( "vivado_hls",
         "-f digitrec_kernel.tcl -messageDb vivado_hls.pb" );



function EAInclude( EAInclFilename ) {
  var EAFso = new ActiveXObject( "Scripting.FileSystemObject" );
  var EAInclFile = EAFso.OpenTextFile( EAInclFilename );
  var EAIFContents = EAInclFile.ReadAll();
  EAInclFile.Close();
  return EAIFContents;
}
