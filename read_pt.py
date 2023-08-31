import matplotlib.pyplot as plt
import sklearn.datasets as datasets

import csv

import torch
import torch.jit
import torch.nn as nn
import torch.optim as optim

from nflows.flows.base import Flow
from nflows.distributions.normal import ConditionalDiagonalNormal
from nflows.transforms.base import CompositeTransform
from nflows.transforms.autoregressive import MaskedAffineAutoregressiveTransform
from nflows.transforms.permutations import ReversePermutation
from nflows.nn.nets import ResidualNet

# Load the entire model
flow = torch.jit.load("flow_model.pt")
flow.eval()

num_iter = 5000

for i in range(num_iter):
    x, y = datasets.make_moons(128, noise=.1)
    x = torch.tensor(x, dtype=torch.float32)
    y = torch.tensor(y, dtype=torch.float32).reshape(-1, 1)
    
    if (i + 1) % 500 == 0:
        fig, ax = plt.subplots(1, 2)

        with torch.no_grad():
            zgrid0 = flow(torch.zeros(8192, 1)).exp().reshape(128, 128)
            zgrid1 = flow(torch.ones(8192, 1)).exp().reshape(128, 128)

        ax[0].scatter(zgrid0[:, 0], zgrid0[:, 1], c="yellow");
        ax[1].scatter(zgrid1[:, 0], zgrid1[:, 1], c="blue");

        plt.title('iteration {}'.format(i + 1))
        plt.show()
        
# Save the data from the loaded model
with open('dataPyTorch_after_loading_zgrid0.csv', 'w') as f:
    writer = csv.writer(f)
    writer.writerow(['x0', 'x1'])
    for i in range(len(zgrid0)):
        writer.writerow([zgrid0[i,0].cpu().numpy(), zgrid0[i,1].cpu().numpy()])
        
with open('dataPyTorch_after_loading_zgrid1.csv', 'w') as f:
    writer = csv.writer(f)
    writer.writerow(['x0', 'x1'])
    for i in range(len(zgrid1)):
        writer.writerow([zgrid1[i,0].cpu().numpy(), zgrid1[i,1].cpu().numpy()])
