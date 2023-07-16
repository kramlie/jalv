// Copyright 2007-2022 David Robillard <d@drobilla.net>
// SPDX-License-Identifier: ISC

#ifndef JALV_STATE_H
#define JALV_STATE_H

#include "attributes.h"
#include "types.h"

#include "lilv/lilv.h"
#include "lv2/atom/atom.h"
#include "lv2/state/state.h"
#include "lv2/urid/urid.h"
#include "serd/serd.h"

JALV_BEGIN_DECLS

// State and preset utilities

typedef int (*PresetSink)(Jalv*           jalv,
                          const LilvNode* node,
                          const LilvNode* title,
                          void*           data);

int
jalv_load_presets(Jalv* jalv, PresetSink sink, void* data);

int
jalv_unload_presets(Jalv* jalv);

int
jalv_apply_preset(Jalv* jalv, const LilvNode* preset);

int
jalv_delete_current_preset(Jalv* jalv);

int
jalv_save_preset(Jalv*       jalv,
                 const char* dir,
                 const char* uri,
                 const char* label,
                 const char* filename);

void
jalv_save(Jalv* jalv, const char* dir);

void
jalv_save_port_values(Jalv* jalv, SerdWriter* writer, const SerdNode* subject);

char*
jalv_make_path(LV2_State_Make_Path_Handle handle, const char* path);

void
jalv_apply_state(Jalv* jalv, LilvState* state);

char*
atom_to_turtle(LV2_URID_Unmap* unmap,
               const SerdNode* subject,
               const SerdNode* predicate,
               const LV2_Atom* atom);

/****************** ZYNTHIAN STUFF *************************/
int
jalv_save_bank_preset(Jalv*  jalv,
                 const char* dir,
                 const char* uri_bank,
                 const char* uri_preset,
                 const char* label_preset,
                 const char* filename);

void
jalv_command_save_preset(Jalv* jalv, char* sym);

int
jalv_fix_symbol(char* symbol, const char* extra_allowed_chars);

int
jalv_fix_filename(char* fname);
/****************** END OF ZYNTHIAN STUFF *************************/

JALV_END_DECLS

#endif // JALV_STATE_H
