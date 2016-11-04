import pymel.core as pm
import maya.cmds as cmds
import fbx_import

class tool(object):
    width = 300
    padded_width = width - 2
    height = 500

    def __init__(self):
        self.create_ui()


    # Starts creation of the UI.
    def create_ui(self):
        if cmds.window("Editor_Window", exists=True):
            cmds.deleteUI("Editor_Window")

        window = cmds.window("Editor_Window", title="Editor Window", widthHeight=(self.width, self.height) )
        tab_layout = cmds.tabLayout()
        
        self.gameobject_inspector = GameObjectInspector(self)
        #self.create_gameobject_inspector()
        
        cmds.setParent('..')

        self.physics_inspector = PhysicsInspector(self)
        #self.create_physics_inspector()
        
        cmds.showWindow( window )


class GameObjectInspector(object):
    last_imported_file = ""
    last_selected_item = ""
    selected_gameobject = None


    def __init__(self, tool):
        self.tool = tool
        self.create_gameobject_inspector()

    # Creates the gameobject inspector UI.
    def create_gameobject_inspector(self):
        inspector_layout = cmds.columnLayout('Gameobject Inspector')
        
        cmds.button(label='Refresh', command=self.refresh_gameobject_list, width=tool.padded_width, height=40)
        # Create and fill gameobject list
        self.create_gameobject_list()

        cmds.button(label='Import gameobject', command=self.import_fbx, width=tool.padded_width, height=40)
        cmds.button(label='Duplicate gameobject', command=self.duplicate_selected_gameobject, width=tool.padded_width, height=40)
        #cmds.button(label='Delete gameobject', command=self.delete_selected_gameobject, width=tool.padded_width, height=40)
        
        # Create name & namespace textfields
        self.name_field = cmds.textField('name_field', text='No gameobject selected.', editable=False, width=tool.padded_width)
        self.filepath_field = cmds.textField('filepath_field', text='File path', editable=False, width=tool.padded_width)

    def import_fbx(self, *_):
        print "Import fbx: " + fbx_import.select_fbx()

    def selected_item_changed(self, *args):
        self.selected_gameobject = self.get_selected_gameobject()
        self.name_field = cmds.textField('name_field', text=self.selected_gameobject, edit=True)
        '''try:
            namespace = cmds.referenceQuery(self.selected_gameobject, filename=True)
            self.filepath_field = cmds.textField('filepath_field', text=namespace, edit=True)
        except RuntimeError as e:
            self.filepath_field = cmds.textField('filepath_field', text=str(e), edit=True)'''

    # Creates the text scroll list.
    def create_gameobject_list(self):
        cmds.textScrollList('gameobjects', selectCommand=self.selected_item_changed, width=tool.padded_width - 4, allowMultiSelection=False, append=self.get_all_gameobjects())

    # Updates the values of the list (for after importing or deleting gameobjects).
    def refresh_gameobject_list(self, *_):
        cmds.textScrollList('gameobjects', edit=True, ra=True)
        cmds.textScrollList('gameobjects', edit=True, append=self.get_all_gameobjects())

    # Returns a list of all referenced nodes.
    def get_all_gameobjects(self):
        local_gameobject_list = []

        all_transform_names = pm.ls(transforms=True, sn=True)
        transform_roots = []

        for transform in all_transform_names:
            if "|" in transform:
                transform_root = transform.split("|")[0]
                if transform_root not in transform_roots:
                    transform_roots.append(transform_root)
                    print transform_root

        return transform_roots

    def get_all_reference_nodes(self):
        # List all nodes
        #all_reference_nodes = pm.ls(type='reference')
        all_nodes = cmds.ls(l=True)
        for obj in all_nodes:
            #print obj + " (" + cmds.nodeType(obj) + ")"
            if(cmds.nodeType(obj) == 'reference'): # Filter for referenced nodes
                print "reference: " + obj
                local_rn_list.append(obj)

        return local_rn_list

    # Duplicates the gameobject linked to the currently selected list item
    def duplicate_selected_gameobject(self, *args):
        filePath = cmds.referenceQuery()
        print "duplicate selected gameobject"

    # Unlocks and deletes the gameobject linked to the currently selected list item.
    def delete_selected_gameobject(self, *args):
        # Unlock and delete children
        '''children = cmds.listRelatives(self.selected_gameobject, children=True)

        for child in children:
            child = self.selected_gameobject + "|" + child
            if pm.lockNode(child, query=True, lock=True):
                pm.lockNode(child, lock=False)'''
            #pm.select(child)
            #pm.delete()
        

        # Unlock node incase it's locked
        if pm.lockNode(self.selected_gameobject, query=True, lock=True):
            pm.lockNode(self.selected_gameobject, lock=False)

        # Select the to be deleted node
        pm.select(self.selected_gameobject)

        # Delete node
        pm.delete()

        # Refresh gameobject list
        self.refresh_gameobject_list()

    # Returns the gameobject linked to the currently selected list item.
    def get_selected_gameobject(self):
        # Get the name of the currently selected list object
        selected_name = pm.textScrollList('gameobjects', query=True, selectItem=True)[0]

        # TODO: pymel 
        # List all nodes
        all_objects = self.get_all_gameobjects()
        for obj in all_objects:
            if obj == selected_name:
                return obj; # Return referenced node with the same name

    def get_selected_reference_node(self):
        # Get the name of the currently selected list object
        selected_name = pm.textScrollList('gameobjects', query=True, selectItem=True)[0]

        # TODO: this won't work because the selected_name has changed from the name of the reference node to the name of the transform
        all_nodes = self.get_all_reference_nodes()
        for node in all_nodes:
            if node == selected_name:
                return node;
                

class GameObject(object):
    name = ""
    reference_node = ""
    reference_path = ""

    def __init__(self, tool):
        self.find_reference_node()

    def find_reference_node(self):
        print "find reference node"

    def get_reference_path(self):
        print "get reference path"

class PhysicsInspector(object):
    shape_text_list = []
    selected_shape = None

    def __init__(self, tool):
        self.tool = tool
        self.create_physics_inspector()

    # Creates the physics inspector UI.
    def create_physics_inspector(self):
        columnLayout2 = cmds.columnLayout('Physics Inspector')
        
        cmds.button(label='Refresh', command=self.refresh_shape_list, width=tool.padded_width, height=40)
        # Create and fill gameobject list
        self.create_shape_list()

        cmds.button(label='Add collision box', command=self.add_attributes, width=tool.padded_width, height=40)
        cmds.button(label='Edit collision box', command=self.edit_attributes, width=tool.padded_width, height=40)
        cmds.button(label='Save position and rotation from scene', command=self.save_scene_values, width=tool.padded_width, height=40)
        cmds.button(label='Remove collision box', command=self.remove_attributes, width=tool.padded_width, height=40)
        cmds.button(label='Show collision box', command=self.show_collision_box, width=tool.padded_width, height=40)
        cmds.button(label='Hide collision box', command=self.hide_collision_box, width=tool.padded_width, height=40)

    def create_shape_list(self):
        cmds.textScrollList('shapes', selectCommand=self.selected_item_changed, width=tool.padded_width - 4, allowMultiSelection=False, append=self.get_all_shapes())

    def refresh_shape_list(self, *_):
        cmds.textScrollList('shapes', edit=True, ra=True)
        cmds.textScrollList('shapes', edit=True, append=self.get_all_shapes())

    def selected_item_changed(self, *args):
        self.selected_shape = self.get_selected_shape()

    # Returns a list of all shape nodes.
    def get_all_shapes(self):
        return cmds.ls(shapes=True)

    # Returns the shape linked to the currently selected list item.
    def get_selected_shape(self):
        # Get the name of the currently selected list item
        selected_name = pm.textScrollList('shapes', query=True, selectItem=True)[0]

        # List all shapes
        all_objects = cmds.ls(shapes=True)
        for obj in all_objects:
            if obj == selected_name:
                return obj; # Return shape node with the same name

    def add_attributes(self, *_):
        parent = cmds.listRelatives(self.selected_shape, type='transform', parent=True)[0]
        cmds.select(parent)
        if(not self.has_attribute(parent, 'collision_size_x')):
            cmds.addAttr(parent, shortName='col_size_x', longName='collision_size_x', attributeType="float")
            cmds.addAttr(parent, shortName='col_size_y', longName='collision_size_y', attributeType="float")
            cmds.addAttr(parent, shortName='col_size_z', longName='collision_size_z', attributeType="float")
            cmds.addAttr(parent, shortName='col_offset_x', longName='collision_offset_x', attributeType="float")
            cmds.addAttr(parent, shortName='col_offset_y', longName='collision_offset_y', attributeType="float")
            cmds.addAttr(parent, shortName='col_offset_z', longName='collision_offset_z', attributeType="float")
            cmds.addAttr(parent, shortName='col_rotation_x', longName='collision_rotation_x', attributeType="float")
            cmds.addAttr(parent, shortName='col_rotation_y', longName='collision_rotation_y', attributeType="float")
            cmds.addAttr(parent, shortName='col_rotation_z', longName='collision_rotation_z', attributeType="float")
            bb = pm.PyNode(parent).getBoundingBox();
            cmds.setAttr(parent + '.col_size_x', bb.width() / 2)
            cmds.setAttr(parent + '.col_size_y', bb.height() / 2)
            cmds.setAttr(parent + '.col_size_z', bb.depth() / 2)
            cmds.setAttr(parent + '.col_rotation_x', cmds.getAttr(parent + '.rotateX'))
            cmds.setAttr(parent + '.col_rotation_y', cmds.getAttr(parent + '.rotateY'))
            cmds.setAttr(parent + '.col_rotation_z', cmds.getAttr(parent + '.rotateZ'))
        '''else:
            cmds.setAttr(parent + '.col_size_x', float(cmds.textField(self.size_x_field, query=True, text=True)))
            cmds.setAttr(parent + '.col_size_y', float(cmds.textField(self.size_y_field, query=True, text=True)))
            cmds.setAttr(parent + '.col_size_z', float(cmds.textField(self.size_z_field, query=True, text=True)))'''

    def save_scene_values(self, *_):
        parent = cmds.listRelatives(self.selected_shape, type='transform', parent=True)[0]

        '''cmds.setAttr(parent + '.col_size_x', cmds.getAttr(parent + '_collision_box.width'))
        cmds.setAttr(parent + '.col_size_y', cmds.getAttr(parent + '_collision_box.height'))
        cmds.setAttr(parent + '.col_size_z', cmds.getAttr(parent + '_collision_box.depth'))'''
        cmds.setAttr(parent + '.col_offset_x', cmds.getAttr(parent + '_collision_box.translateX'))
        cmds.setAttr(parent + '.col_offset_y', cmds.getAttr(parent + '_collision_box.translateY'))
        cmds.setAttr(parent + '.col_offset_z', cmds.getAttr(parent + '_collision_box.translateZ'))
        cmds.setAttr(parent + '.col_rotation_x', cmds.getAttr(parent + '_collision_box.rotateX'))
        cmds.setAttr(parent + '.col_rotation_y', cmds.getAttr(parent + '_collision_box.rotateY'))
        cmds.setAttr(parent + '.col_rotation_z', cmds.getAttr(parent + '_collision_box.rotateZ'))

    def edit_attributes(self, *_):
        parent = cmds.listRelatives(self.selected_shape, type='transform', parent=True)[0]
        cmds.select(parent)
        # Set the right focus in the Attribute Editor
        pm.melGlobals.initVar('string', 'gAETabLayoutName')
        tabs = pm.tabLayout(pm.melGlobals['gAETabLayoutName'], q=1, tabLabelIndex=1)
        for i in range(0,len(tabs)):
            if tabs[i] == str(parent):
                pm.tabLayout(pm.melGlobals['gAETabLayoutName'], selectTabIndex=(i + 1), e=True)

    def has_attribute(self, node, attribute_name):
        #return cmds.attributeQuery(attribute_name, node=node, exists=True)
        # Both ways work
        node_attr = cmds.listAttr(node)

        for attr in node_attr:
            if attr == attribute_name:
                return True;
        
        return False;

    def remove_attributes(self, *_):
        parent = cmds.listRelatives(self.selected_shape, type='transform', parent=True)[0]
        cmds.deleteAttr(parent + '.col_size_x')
        cmds.deleteAttr(parent + '.col_size_y')
        cmds.deleteAttr(parent + '.col_size_z')
        cmds.deleteAttr(parent + '.col_offset_x')
        cmds.deleteAttr(parent + '.col_offset_y')
        cmds.deleteAttr(parent + '.col_offset_z')
        cmds.deleteAttr(parent + '.col_rotation_x')
        cmds.deleteAttr(parent + '.col_rotation_y')
        cmds.deleteAttr(parent + '.col_rotation_z')

    def show_collision_box(self, *_):
        parent = cmds.listRelatives(self.selected_shape, type='transform', parent=True)[0]
        w = cmds.getAttr(parent + '.col_size_x') * 2
        h = cmds.getAttr(parent + '.col_size_y') * 2
        d = cmds.getAttr(parent + '.col_size_z') * 2
        offset_x = cmds.getAttr(parent + '.col_offset_x')
        offset_y = cmds.getAttr(parent + '.col_offset_y')
        offset_z = cmds.getAttr(parent + '.col_offset_z')
        rotation_x = cmds.getAttr(parent + '.col_rotation_x')
        rotation_y = cmds.getAttr(parent + '.col_rotation_y')
        rotation_z = cmds.getAttr(parent + '.col_rotation_z')
        cube = cmds.polyCube(name=parent + '_collision_box', w=w, h=h, d=d)
        cmds.parent(cube, parent)
        cmds.setAttr(str(cube[0]) + 'Shape.displayEdges', 2)
        cmds.setAttr(str(cube[0]) + '.translateX', offset_x)
        cmds.setAttr(str(cube[0]) + '.translateY', offset_y)
        cmds.setAttr(str(cube[0]) + '.translateZ', offset_z)
        cmds.setAttr(str(cube[0]) + '.rotateX', rotation_x)
        cmds.setAttr(str(cube[0]) + '.rotateY', rotation_y)
        cmds.setAttr(str(cube[0]) + '.rotateZ', rotation_z)

    def hide_collision_box(self, *_):
        parent = cmds.listRelatives(self.selected_shape, type='transform', parent=True)[0]
        relatives = cmds.listRelatives(parent, children=True)

        for relative in relatives:
            print relative
            if relative == parent + "_collision_box":
                cmds.delete(relative)
                return



tool()