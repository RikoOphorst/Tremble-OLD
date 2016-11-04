import maya.cmds as cmds

# Opens a selection dialog.
def select_fbx():
    # Open file dialog
    filename = cmds.fileDialog2(fileFilter="*.fbx", fileMode=1, caption="Reference FBX", dialogStyle=2)
    
    # Extract file name from full path
    actual_name = get_actual_name(filename[0])
    
    '''
    # Loop through existing references to check if a reference to this object already exists
    references = cmds.ls(references=True)
    
    if len(references) > 0:
        for ref in references:
            ref_name = cmds.referenceQuery(ref, filename=True)
            
            if ref_name[0] == filename[0]:
                print "Reference: " + ref_name[0] + " equals " + filename[0]
                rn = cmds.referenceQuery(ref, referenceNode=True)
                cmds.instance(rn)
                return
    '''
    # Create a reference to the selected FBX
    create_reference(filename=filename[0], actual_name=actual_name)
    return filename[0]

# Creates a reference to the specified file.
def create_reference(filename, actual_name):
    cmds.file(filename, r=True, options="fbx", namespace=actual_name, mergeNamespacesOnClash=True)

# Extract file name from full path.
def get_actual_name(filename):
    actual_name = filename.split('/')
    actual_name = actual_name[len(actual_name) - 1][:-4]
    return actual_name