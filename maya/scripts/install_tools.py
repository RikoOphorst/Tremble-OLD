import sys
import maya.cmds as cmds
import maya.mel as mel

def install_tools():
	set_grid_snapping()
	create_tool_menu()
	run_inspector()
	#set_interactive_creation()

def set_grid_snapping():
	cmds.snapMode(grid=True)

def set_interactive_creation():
	'''mel.eval('toggleCreatePolyPrimitiveAsTool')
	menu_items = cmds.lsUI(menuItems=True)

	for ui in menu_items:
		label = cmds.menuItem(ui, q=True, label=True)
		if "Interactive Creation" in label or 'polyPrimitivesItem' in ui:
			print ui'''

	try:
		checked = mel.eval('menuItem -q -checkBox "toggleCreatePolyPrimitivesAsToolItem"')
		if checked == 1: #cmds.menuItem('toggleCreatePolyPrimitivesAsToolItem', q=True, checkBox=True) == 1:
			print "Interactive Creation is already enabled"
			return
	except RuntimeError as e:
		print "Interactive Creation menu item can't be accessed. Please enable \nthis manually (Create>Polygon Primitives>Interactive Creation)"

	print "Toggling Interactive Creation"
	mel.eval('toggleCreatePolyPrimitiveAsTool')


def create_tool_menu():
	try: # Check if menu already exists and delete it if it does.
		cmds.menu('MayaWindow|levelEditorMenu', q=True, label=True)

		print "A tool menu already exists, which will now be deleted."
		# Delete existing tool menu.
		cmds.deleteUI('MayaWindow|levelEditorMenu', menu=True)
	except RuntimeError as e:
		print "No tool menu exists yet."

	print "Creating new tool menu."
	g_main_window = mel.eval('$temp1 = $gMainWindow')
	# Create new tool menu in the main window.
	custom_menu = cmds.menu('levelEditorMenu', parent = g_main_window, label="Level Editor")

	# Create tool menu items.
	cmds.menuItem(parent = custom_menu, label="Run Inspector", command=run_inspector)
	cmds.menuItem(parent = custom_menu, label="Open Commandports", command=open_ports)

# Opens the inspector tool.
def run_inspector(*_):
	import tool
	tool.tool()

# Opens the command ports for communication with external programs such as SublimeText.
def open_ports(*_):
	import open_command_ports
	open_command_ports.open_ports()