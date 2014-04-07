bl_info = {
    "name":         "Scene exporter for derelict factory",
    "author":       "Henrik Paananen",
    "blender":      (2, 69, 0),
    "version":      (0, 0, 1),
    "location":     "File > Import-Export",
    "description":  "Export DER Scene",
    "category":     "Import-Export"
}

import bpy

from bpy.props import StringProperty, BoolProperty, EnumProperty, FloatProperty
import mathutils

import time
import struct

ObjectType = { 'MESH': 0, 'LAMP': 1 }

class DerSceneExport(bpy.types.Operator):
  bl_idname  = "export_der_scene.derscene";
  bl_label   = "Export DER Scene";
  bl_options = {'PRESET'};

  filepath = StringProperty(
    name = "File Path",
    description = "Filepath used for exporting the file",
    maxlen = 1024,
    subtype = 'FILE_PATH')
  check_existing = BoolProperty(
    name = "Check Existing",
    description = "Check and warn on overwriting existing files",
    default = True,
    options = {'HIDDEN'})

  filename_ext = ".derscene";


  def execute(self, context):
    t = time.clock()

    self.filepath = bpy.path.ensure_ext(self.filepath, self.filename_ext)

    with open(self.filepath, 'wb') as out:
      self._export(out, context)

    t = time.clock() - t
    print("Done in {:.2f} seconds".format(t))

    return {'FINISHED'};

  def invoke(self, context, event):
    if not self.filepath:
      self.filepath = bpy.path.ensure_ext(bpy.data.filepath, self.filename_ext)
    context.window_manager.fileselect_add(self)
    return {'RUNNING_MODAL'}

  def _export(self, out, context):
    for obj in context.scene.objects:
      if obj.type == 'MESH':
        self._export_mesh(out, obj)
      if obj.type == 'LAMP':
        self._export_lamp(out, obj)

  def _write_object(self, out, obj):
    out.write(struct.pack('I', ObjectType[obj.type]))
    self._write_transform(out, obj.matrix_world)

  def _write_name(self, out, name):
    out.write(struct.pack('32s', bytes(name, 'ascii')))

  def _write_transform(self, out, mat):
    # might need a basis change
    out.write(struct.pack('4f', mat[0][0], mat[0][1], mat[0][2], mat[0][3]))
    out.write(struct.pack('4f', mat[1][0], mat[1][1], mat[1][2], mat[1][3]))
    out.write(struct.pack('4f', mat[2][0], mat[2][1], mat[2][2], mat[2][3]))
    # no need to write the row 0 0 0 1
    #out.write(struct.pack('4f', mat[3][0], mat[3][1], mat[3][2], mat[3][3]))

  def _export_mesh(self, out, obj):
    self._write_object(out, obj)
    self._write_name(out, obj.data.name)
    self._write_name(out, obj.active_material.name)

  def _export_lamp(self, out, obj):
    pass


def menu_func(self, context):
  self.layout.operator(DerSceneExport.bl_idname, text="DER Scene(.derscene)")

def register():
  bpy.utils.register_module(__name__)
  bpy.types.INFO_MT_file_export.append(menu_func)


def unregister():
  bpy.utils.unregister_module(__name__)
  bpy.types.INFO_MT_file_export.remove(menu_func)


if __name__ == "__main__":
  register()
