importPackage(Packages.com.ti.debug.engine.scripting);
importPackage(Packages.com.ti.ccstudio.scripting.environment);
importPackage(Packages.java.lang);
importPackage(Packages.java.io);

ccxmlPath = arguments[0];
binary = arguments[1];
core = arguments[2];

var script = ScriptingEnvironment.instance();
script.traceWrite("Begin scripting session");

script.traceWrite("Setting config...");
var debugServer = script.getServer("DebugServer.1");
debugServer.setConfig(ccxmlPath);
script.traceWrite("Config set");

script.traceWrite("Connecting R50...");
debugSession_r50 = debugServer.openSession(core);
debugSession_r50.target.connect();
script.traceWrite("Connected");

script.traceWrite("Loading binary...");
debugSession_r50.memory.loadProgram(binary);
script.traceWrite("Loaded");

script.traceWrite("Run...");
//debugSession_r50.target.run();
debugSession_r50.target.runAsynch();
//debugSession_r50.target.halt();
script.traceWrite("Complete");

debugSession_r50.target.disconnect();
script.traceWrite("Disconnected");

debugSession_r50.terminate();

