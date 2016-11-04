#list all objects
all_objects = cmds.ls(l=True)
for obj in all_objects:
    if(cmds.nodeType(obj) == 'reference'):
        print obj + " deleted."
        cmds.delete(obj)