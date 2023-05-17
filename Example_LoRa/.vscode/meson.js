//import * as logging from '@cmt/logging';
const vscode = require("vscode");

exports.execute = async (args) => {
    var ret = null;
    switch (args.command)
    {
        case 'otx.preLaunch':
            ret = await otxBuild(); 
            break;
        case 'otx.clean':
            ret = await otxClean();
            break;
        case 'otx.build':
            ret = await otxBuild();
            break;
        case 'otx.run':
            ret = await otxRun();
            break;
    }
    return ret;
};

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

function getDate() {
    const d_t = new Date();
    let year = d_t.getFullYear();
    let month = ("0" + (d_t.getMonth() + 1)).slice(-2);
    let day = ("0" + d_t.getDate()).slice(-2);
    let hour = ("0" + d_t.getHours()).slice(-2);
    let minute = ("0" + d_t.getMinutes()).slice(-2);
    let seconds = ("0" + d_t.getSeconds()).slice(-2);
    return year + "-" + month + "-" + day + "_" + hour + "-" + minute + "-" + seconds;
}

// otxPreLaunch = async () => {
//     var ret = await executeTask("Meson: build");
//     if (ret == 0) return '';
//     vscode.window.showErrorMessage("The build task terminated with exit code:" + JSON.stringify(ret));
//     vscode.commands.executeCommand('workbench.action.problems.focus');
//     //await vscode.commands.executeCommand('workbench.panel.output.focus', 'Adapter Output');
//     return null;
// };

otxClean = async () => {
    let { status, message, basePath } = await checkSetup();
    var buildFolder = path.join(basePath, "build");
    var ret = 0;
    if (status == 'error')
    {
        vscode.window.showErrorMessage(message);
        return null;
    }
    else if (status == 'missing') await fs.promises.mkdir(buildFolder);
    else
    {
        var backupFolder = path.join(buildFolder, "backup");
        var nowFolder = path.join(backupFolder, getDate());
        if (!fs.existsSync(backupFolder)) await fs.promises.mkdir(backupFolder);
        fs.readdirSync(buildFolder).forEach(file => {
            let current = path.join(buildFolder, file);
            if (fs.statSync(current).isFile()) {
                if(current.endsWith(".elf") || current.endsWith(".hex") || current.endsWith(".txt") || current.endsWith(".json")) {
                    if (!fs.existsSync(nowFolder)) fs.promises.mkdir(nowFolder);
                    var destFile =  path.join(nowFolder, file);
                    fs.promises.copyFile(current, destFile); 
                }
            } 
            if (file != 'backup')
               fs.rmSync(current, { recursive: true, force: true });
        });
    }

	const mesonBuildFile = path.join(basePath, "meson.build");
    await updateMeson(mesonBuildFile, [], []);
    ret = await executeTask("Meson: configure");
    if (ret == 0) return '';
    vscode.window.showErrorMessage("The build task terminated with exit code:" + JSON.stringify(ret));
    return null;
};

otxBuild = async () => {
    let { status, message, basePath } =  await checkSetup();
    if (status != 'ok') 
    {
        vscode.window.showErrorMessage("The build task terminated with exit code: " + status + "\r\nPlease Clean-Reconfigure.", { modal: true });
        return null;
    }
    let sourcePath = path.join(basePath, "source");
    const mesonBuildFile = path.join(basePath, "meson.build");
    if (!fs.existsSync(mesonBuildFile))
    {
        vscode.window.showErrorMessage("meson.build file not found!");
        return null;
    }
    var headerContents = readDirectory(basePath, [], sourcePath, '.h', true);
    var sourceContents = readDirectory(basePath, [], sourcePath, '.c', false);

    updateMeson(mesonBuildFile, headerContents, sourceContents);
    var ret = await executeTask("Meson: build");
    if (ret == 0) return '';
    vscode.window.showErrorMessage("The build task terminated with exit code:" + JSON.stringify(ret));
    vscode.commands.executeCommand('workbench.action.problems.focus');
    //await vscode.commands.executeCommand('workbench.panel.output.focus', 'Adapter Output');
    return null;
}

async function executeTask(taskName)
{   
    var tasks = await vscode.tasks.fetchTasks();
    var task = undefined;
    for (var t of tasks) {
        if (t.name === taskName) task = t;
    }
    if (task === undefined)
    {
        vscode.window.showErrorMessage("Can not find " + taskName + " task.");
        return;
    }
    const taskExecution = await vscode.tasks.executeTask(task);
    return new Promise((resolve) => {
        vscode.tasks.onDidEndTaskProcess(e => {
            if (e.execution === taskExecution || e.execution.task === task) 
                resolve(e.exitCode);
        });
    });
}

otxRun = async () => {
    //var ret = await vscode.commands.executeCommand('workbench.action.debug.run');
    //var ret = await vscode.commands.executeCommand('workbench.action.debug.selectandstart');
    var ret = await vscode.commands.executeCommand('workbench.action.debug.start');
    //vscode.treeView.reveal('Run', {focus: true});
    //vscode.commands.executeCommand('workbench.action.output.focus');
    vscode.commands.executeCommand('workbench.debug.action.focusRepl');
    return ret;
};

const fs = require("fs");
const path = require("path");

async function checkSetup()
{
    var basePath = "";
    if(vscode.workspace.workspaceFolders === undefined)
    {
        vscode.window.showErrorMessage("No workspace opened!");
        return { 'status': 'error', 'message': "No workspace opened!", 'basePath': basePath };
    }
    basePath = vscode.workspace.workspaceFolders[0].uri.fsPath;
    buildDir = path.join(basePath, "build");
    if (!fs.existsSync(buildDir)) return { 'status': 'missing', 'message': "Missing Build Folder", 'basePath': basePath };
    if (!fs.existsSync(path.join(buildDir, "meson-private"))) return { 'status': 'unconfigured', 'message': "Unconfigured Build Folder", 'basePath': basePath };
    if (!fs.existsSync(path.join(buildDir, "meson-info"))) return { 'status': 'unconfigured', 'message': "Unconfigured Build Folder", 'basePath': basePath };
    if (!fs.existsSync(path.join(buildDir, "meson-logs"))) return { 'status': 'unconfigured', 'message': "Unconfigured Build Folder", 'basePath': basePath };
    if (!fs.existsSync(path.join(buildDir, "build.ninja"))) return { 'status': 'unconfigured', 'message': "Unconfigured Build Folder", 'basePath': basePath };
    if (!fs.existsSync(path.join(buildDir, "compile_commands.json"))) return { 'status': 'unconfigured', 'message': "Unconfigured Build Folder", 'basePath': basePath };
    return { 'status': 'ok', 'message': "OK", 'basePath': basePath };
}

function writeFile(fileName, contents)
{
	fs.writeFile(fileName, contents, (err) => {
        if (err) throw err;
    })
}

function readDirectory(basePath, refArray, dir, extension, foldersOnly) {
	var pushed = false;
	fs.readdirSync(dir).forEach(file => {
		let current = path.join(dir,file);
		if (fs.statSync(current).isFile()) {
			if(current.endsWith(extension)) {
				if (foldersOnly) {
                    var fle = path.relative(basePath, dir).replaceAll('\\', '/');
					if (!pushed) refArray.push('\t\'' + fle + '\',');
					pushed = true;
				}
				else
                {
                    var fle = path.relative(basePath, current).replaceAll("\\", "/");
					refArray.push('\t\'' + fle + '\',');
                }
			} 
		} else
			readDirectory(basePath, refArray, current, extension, foldersOnly)
	});
    return refArray;
}

var process = require('process');
function updateMeson(mesonFile, headerContents, sourceContents) {
    const mesonContents = fs.readFileSync(mesonFile, 'utf-8');
	var arr = [];
	var logOut = true; var linesStripped = 0;
	mesonContents.split(/\r?\n/).forEach((line) => {
		if (line.includes("OTX_Extension_HeaderFiles_End") || line.includes("OTX_Extension_SourceFiles_End")) logOut = true;
		if (logOut) arr.push(line);
        if (linesStripped > 0 && --linesStripped == 0) logOut = true;
		if (line.includes("OTX_Extension_HeaderFiles_Start")) {
			arr = arr.concat(headerContents);
			logOut = false;
		}
		else if (line.includes("OTX_Extension_SourceFiles_Start")) {
			arr = arr.concat(sourceContents);
			logOut = false;
		}
        else if (line.includes("OTX_Extension_print")) {
            const regexp = /\(\s*(.*[^ ])[ )]+$/;
            var array = line.match(regexp);
            if (array != null)
                arr = arr.concat(substituteVariables(array[1]));
            else
                arr = arr.concat('Not found!');
			logOut = false; linesStripped = 1;
		}
	});
	const contents= arr.join('\n');
	//console.log(contents);
	if(contents == mesonContents) return;
	writeFile(mesonFile, contents);
}


// https://raw.githubusercontent.com/usernamehw/vscode-commands/master/src/substituteVariables.ts
// https://github.com/microsoft/vscode/blob/main/src/vs/workbench/services/configurationResolver/common/variableResolver.ts

// TODO: ${userHome}
const VariableNames = {
	File : '${file}', // the current opened file (absolute path?)
	FileBasename : '${fileBasename}', // the current opened file's basename
	FileBasenameNoExtension : '${fileBasenameNoExtension}', // the current opened file's basename with no file extension
	FileExtname : '${fileExtname}', // the current opened file's extension
	FileDirname : '${fileDirname}', // the current opened file's dirname
	FileWorkspaceFolder : '${fileWorkspaceFolder}', // the current opened file's workspace folder
	WorkspaceFolder : '${workspaceFolder}', // the path of the folder opened in VS Code
	WorkspaceFolderBasename : '${workspaceFolderBasename}', // the name of the folder opened in VS Code without any slashes (/)
	ExecPath : '${execPath}', //  location of Code.exe
	PathSeparator : '${pathSeparator}', // `/` on macOS or linux, `\` on Windows
	LineNumber : '${lineNumber}', // the current selected line number in the active file
	SelectedText : '${selectedText}', // the current selected text in the active file
	EnvironmentVariable : '${env}',
	SingleEnvironmentVariable : 'env',
	ConfigurationVariable : '${config}',
	SingleConfigurationVariable : 'config',
	// ────────────────────────────────────────────────────────────
	// relativeFile : '${relativeFile}', // the current opened file relative to `workspaceFolder`
	// relativeFileDirname : '${relativeFileDirname}', // the current opened file's dirname relative to `workspaceFolder`
	// cwd : '${cwd}', // the task runner's current working directory on startup
}

const variableRegexps = {
	[VariableNames.File]: new RegExp(escapeRegExp(VariableNames.File), 'ig'),
	[VariableNames.FileBasename]: new RegExp(escapeRegExp(VariableNames.FileBasename), 'ig'),
	[VariableNames.FileBasenameNoExtension]: new RegExp(escapeRegExp(VariableNames.FileBasenameNoExtension), 'ig'),
	[VariableNames.FileDirname]: new RegExp(escapeRegExp(VariableNames.FileDirname), 'ig'),
	[VariableNames.FileExtname]: new RegExp(escapeRegExp(VariableNames.FileExtname), 'ig'),
	[VariableNames.FileWorkspaceFolder]: new RegExp(escapeRegExp(VariableNames.FileWorkspaceFolder), 'ig'),
	[VariableNames.WorkspaceFolder]: new RegExp(escapeRegExp(VariableNames.WorkspaceFolder), 'ig'),
	[VariableNames.WorkspaceFolderBasename]: new RegExp(escapeRegExp(VariableNames.WorkspaceFolderBasename), 'ig'),
	[VariableNames.ExecPath]: new RegExp(escapeRegExp(VariableNames.ExecPath), 'ig'),
	[VariableNames.PathSeparator]: new RegExp(escapeRegExp(VariableNames.PathSeparator), 'ig'),
	[VariableNames.LineNumber]: new RegExp(escapeRegExp(VariableNames.LineNumber), 'ig'),
	[VariableNames.SelectedText]: new RegExp(escapeRegExp(VariableNames.SelectedText), 'ig'),
	[VariableNames.SingleEnvironmentVariable]: /\${env:([a-zA-Z_]+[a-zA-Z0-9_]*)}/i,
	[VariableNames.EnvironmentVariable]: /\${env:([a-zA-Z_]+[a-zA-Z0-9_]*)}/ig,
	[VariableNames.SingleConfigurationVariable]: /\${config:([^}]+?)}/i,
	[VariableNames.ConfigurationVariable]: /\${config:([^}]+?)}/ig,
	// [VariableNames.relativeFile]: new RegExp(escapeRegExp(VariableNames.relativeFile), 'ig'),
	// [VariableNames.relativeFileDirname]: new RegExp(escapeRegExp(VariableNames.relativeF
};

function escapeRegExp(string) {
    return string.replace(/[/\-\\^$*+?.()|[\]{}]/g, '\\$&');
}
/**
 * Try to emulate variable substitution in tasks https://code.visualstudio.com/docs/editor/variables-reference
 *
 * TODO: throw errors (window.showMessage) when variable exists but can't resolve
 */

function substituteVariables2(str) {
    const activeTextEditor = vscode.window.activeTextEditor;
    const workspaceFolder = vscode.workspace.workspaceFolders?.[0].uri.fsPath;
    if (str.includes(VariableNames.WorkspaceFolder) && workspaceFolder) {
		str = str.replace(variableRegexps[VariableNames.WorkspaceFolder], workspaceFolder);
	}
    return str;
}
function substituteVariables(str) {
	const activeTextEditor = vscode.window.activeTextEditor;
	const workspaceFolder = vscode.workspace.workspaceFolders?.[0].uri.fsPath;
	if (str.includes(VariableNames.SelectedText) && activeTextEditor) {
		const selection = activeTextEditor.selection;
		const selectedText = activeTextEditor.document.getText(selection);
		str = str.replace(variableRegexps[VariableNames.SelectedText], selectedText);
	}
	if (str.includes(VariableNames.PathSeparator)) {
		str = str.replace(variableRegexps[VariableNames.PathSeparator], path.sep);
	}
	if (str.includes(VariableNames.LineNumber) && activeTextEditor) {
		str = str.replace(variableRegexps[VariableNames.LineNumber], String(activeTextEditor.selection.active.line + 1));
	}
	if (str.includes(VariableNames.ExecPath)) {
		str = str.replace(variableRegexps[VariableNames.ExecPath], vscode.env.appRoot);
	}
	if (str.includes(VariableNames.File) && activeTextEditor) {
		str = str.replace(variableRegexps[VariableNames.File], activeTextEditor.document.uri.fsPath);
	}
	if (str.includes(VariableNames.FileBasename) && activeTextEditor) {
		str = str.replace(variableRegexps[VariableNames.FileBasename], path.basename(activeTextEditor.document.uri.fsPath));
	}
	if (str.includes(VariableNames.FileBasenameNoExtension) && activeTextEditor) {
		str = str.replace(variableRegexps[VariableNames.FileBasenameNoExtension], path.basename(activeTextEditor.document.uri.fsPath, path.extname(activeTextEditor.document.uri.fsPath)));
	}
	if (str.includes(VariableNames.FileExtname) && activeTextEditor) {
		str = str.replace(variableRegexps[VariableNames.FileExtname], path.extname(activeTextEditor.document.uri.fsPath));
	}
	if (str.includes(VariableNames.FileDirname) && activeTextEditor) {
		str = str.replace(variableRegexps[VariableNames.FileDirname], path.dirname(activeTextEditor.document.uri.fsPath));
	}
	if (str.includes(VariableNames.WorkspaceFolder) && workspaceFolder) {
		str = str.replace(variableRegexps[VariableNames.WorkspaceFolder], workspaceFolder);
	}
	if (str.includes(VariableNames.WorkspaceFolderBasename) && workspaceFolder) {
		str = str.replace(variableRegexps[VariableNames.WorkspaceFolderBasename], path.basename(workspaceFolder));
	}
	if (str.includes(VariableNames.FileWorkspaceFolder) && activeTextEditor && workspaceFolder) {
		const fileWorkspaceFolder = vscode.workspace.getWorkspaceFolder(activeTextEditor.document.uri)?.uri.fsPath;
		if (fileWorkspaceFolder) {
			str = str.replace(variableRegexps[VariableNames.FileWorkspaceFolder], fileWorkspaceFolder);
		}
	}
	if (variableRegexps[VariableNames.EnvironmentVariable].test(str)) {
		const match = str.match(variableRegexps[VariableNames.EnvironmentVariable]);

		for (const _ of match || []) {
			str = str.replace(variableRegexps[VariableNames.SingleEnvironmentVariable], (__, g1) => process.env[g1] || g1);
		}
	}
	if (variableRegexps[VariableNames.ConfigurationVariable].test(str)) {
		const match = str.match(variableRegexps[VariableNames.ConfigurationVariable]);

		for (const _ of match || []) {
			str = str.replace(variableRegexps[VariableNames.SingleConfigurationVariable], (__, g1) => replaceConfigurationVariable(g1));
		}
	}
	return str;
}

function replaceConfigurationVariable(configName) {
	if (!configName.includes('.')) {
		vscode.window.showErrorMessage(`Need a dot (.) in the name of configuration. "${configName}"`);
		return configName;
	}
	const configParts = configName.split('.');
	const configValue = vscode.workspace.getConfiguration(configParts[0]).get(configParts.slice(1).join('.'));
	if (typeof configValue !== 'string' && typeof configValue !== 'number') {
		vscode.window.showErrorMessage(`Configuration must be of type: string or number "${configName}"`);
		return configName;
	}
	return String(configValue);
}

/**
 * Walk recursively through object/array and replace variables in strings.
 */
function substituteVariableRecursive(arg) {
	if (typeof arg === 'string') {
		return substituteVariables(arg);
	}

	if (Array.isArray(arg)) {
		for (const [key, value] of arg.entries()) {
			arg[key] = substituteVariableRecursive(value);
		}
	} else if (typeof arg === 'object' && arg !== null) {
		for (const key in arg) {
			// @ts-ignore
			arg[key] = substituteVariableRecursive(arg[key]);
		}
	}

	return arg;
}