import uproot
import awkward as ak
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import matplotlib.colors as colors

import os
from particle import Particle
import networkx as nx
from networkx.drawing.nx_agraph import graphviz_layout

color_in_energy = False

# Load file and tree
fin = '/home/tasheng/tpc/test_xs_wphact211ncgg_e182.7_m80.4_c97_1l_g1_evt100_ttree.root'
# fin = '/home/tasheng/tpc/test_xs_qedbk23eegg_e183.5_l97_1l_g1_evt100_ttree.root'
# fin = '/home/tasheng/tpc/test_xs_gpym6143wc0eeqq_e182.7_c97_1l_g1_evt100_ttree.root'
# fin = '/home/tasheng/tpc/test_xs_wphact24cc_e182.7_m80.4_c97_1l_g1_evt100_ttree.root'

file = uproot.open(fin)
tree = file["t"]
branch_names = tree.keys()

n = 50

# Load branches
arrays = tree.arrays([
    "Elid_tag", "Elid_partIdx",
    "Muid_tag", "Muid_partIdx",
    "Part_pdgId", "Part_simIdx",
    "Part_charge",
    "SimPart_genIdx",
    "GenPart_pdgId", "GenPart_vector",
    "GenPart_vector.fCoordinates.fX",
    "GenPart_vector.fCoordinates.fY",
    "GenPart_vector.fCoordinates.fZ",
    "GenPart_vector.fCoordinates.fT",
    "GenPart_parentIdx",
    'GenPart_firstChildIdx',
    'GenPart_lastChildIdx',
    'GenPart_status',
])


gen_parent = arrays["GenPart_parentIdx"] - 1
gen_pdg = arrays["GenPart_pdgId"]
gen_st = arrays["GenPart_status"]
gen_childi = arrays["GenPart_firstChildIdx"] - 1
gen_childf = arrays["GenPart_lastChildIdx"] - 1

# Compute GenPart pt
gen_vec = arrays["GenPart_vector"]
gen_px = arrays["GenPart_vector.fCoordinates.fX"]
gen_py = arrays["GenPart_vector.fCoordinates.fY"]
gen_pz = arrays["GenPart_vector.fCoordinates.fZ"]
gen_e = arrays["GenPart_vector.fCoordinates.fT"]

mutag = arrays['Muid_tag']
muidx = arrays['Muid_partIdx']
eltag = arrays['Elid_tag']
elidx = arrays['Elid_partIdx']

simidx = arrays['Part_simIdx']
genidx = arrays['SimPart_genIdx']
ch = arrays['Part_charge']

def pdg_to_name(pdgid):
    try:
        return Particle.from_pdgid(pdgid).name
    except Exception:
        return str(pdgid)  # fallback for unknown

def build_decay_graph_named(parent, pdg, status, mass, energy):
    G = nx.DiGraph()
    for i in range(len(pdg)):
        if pdg[i] == 0:
            continue
        name = pdg_to_name(pdg[i])
        G.add_node(i,
                   label=name + f'/{status[i]}\n(E{energy[i]:.0f}/M{mass[i]:.02g})',
                   energy=energy[i])
        if parent[i] >= 0:
            G.add_edge(parent[i], i)
    return G

def draw_and_save(G, event_id, out_dir="decay_trees"):
    os.makedirs(out_dir, exist_ok=True)
    pos = graphviz_layout(G, prog="dot")

    labels = nx.get_node_attributes(G, 'label')
    energies = nx.get_node_attributes(G, 'energy')

    energy_vals = list(energies.values())
    norm = colors.Normalize(vmin=0, vmax=110)
    cmap = cm.inferno  # or any perceptually uniform map

    node_colors = [cmap(norm(90)) for _ in G.nodes()]
    if (color_in_energy):
        node_colors = [cmap(norm(energies[n])) for n in G.nodes()]

    # Convert RGBA to luminance to set contrasting font color
    def is_bright(rgba):
        r, g, b, _ = rgba
        return (0.299 * r + 0.587 * g + 0.114 * b) > 0.3

    font_colors = ['black' if is_bright(c) else 'white' for c in node_colors]

    plt.figure(figsize=(20, 8))
    for color in set(font_colors):
        subnodes = [n for n, fc in zip(G.nodes(), font_colors) if fc == color]
        nx.draw_networkx_nodes(G, pos, nodelist=subnodes,
                               node_color=[node_colors[n] for n in subnodes],
                               node_size=2000)
        nx.draw_networkx_labels(G, pos, labels={n: labels[n] for n in subnodes},
                                font_size=8, font_color=color)
    nx.draw_networkx_edges(G, pos)

    plt.title(f"Gen Particle Decay Tree - Event {event_id}")
    plt.tight_layout()
    plt.savefig(f"{out_dir}/event_{event_id:04d}.png")
    plt.close()

gen_mass = np.sqrt(gen_e ** 2 - gen_px ** 2 - gen_py ** 2 - gen_pz ** 2)
for i in range(n):
    G = build_decay_graph_named(gen_parent[i], gen_pdg[i], gen_st[i],
                                gen_mass[i], gen_e[i])
    draw_and_save(G, i, "decay_tree_" +
                   "".join(fin.split('/')[-1].split('.')[:-1]))
