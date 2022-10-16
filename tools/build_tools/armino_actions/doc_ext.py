import os
import subprocess
from armino_doc import build_doc

def action_extensions(base_actions, project_path):
	def doc_target(target_name, ctx, args):
		armino_path = os.environ['ARMINO_PATH']
		target = os.getenv('temp_soc', "all")
		if (target == "all"):
			build_doc("all")
		else:
			build_doc(target)

	doc_actions = {
		'actions': {
			'doc': {
				'callback': doc_target,
				'short_help': 'Generate the ARMINO docs',
				'dependencies': None,
			},
		}
	}

	return doc_actions
