#!/usr/bin/env python
# coding: utf-8

# In[1]:

import matplotlib.pyplot as plt
import sklearn.datasets as datasets

import torch.utils.model_zoo as model_zoo

import csv

import torch
import torch.jit
import torch.testing

from torch import nn
from torch import optim

from nflows.flows.base import Flow
from nflows.distributions.normal import ConditionalDiagonalNormal
from nflows.transforms.base import CompositeTransform
from nflows.transforms.autoregressive import MaskedAffineAutoregressiveTransform
from nflows.transforms.permutations import ReversePermutation
from nflows.nn.nets import ResidualNet

# Model wrapper

class model_from_flow(torch.nn.Module):
    def __init__(self, flow):
        super().__init__()  
        self.flow = flow

    def forward(self, x):
        self.flow.eval()
        output_dist = self.flow.sample(num_samples=1, context=x)
        return output_dist


# In[2]:


x, y = datasets.make_moons(128, noise=.1)
plt.scatter(x[:, 0], x[:, 1], c=y);

with open('dataPyTorch.csv', 'w') as f:
    writer = csv.writer(f)
    writer.writerow(['x0', 'x1', 'y'])
    for i in range(len(x)):
        writer.writerow([x[i,0], x[i,1], y[i]])


# In[3]:


num_layers = 5
base_dist = ConditionalDiagonalNormal(shape=[2], 
                                      context_encoder=nn.Linear(1, 4))

transforms = []
for _ in range(num_layers):
    transforms.append(ReversePermutation(features=2))
    transforms.append(MaskedAffineAutoregressiveTransform(features=2, 
                                                          hidden_features=4, 
                                                          context_features=1))
transform = CompositeTransform(transforms)

flow = Flow(transform, base_dist)
flow.eval()
optimizer = optim.Adam(flow.parameters())

# In[4]:

num_iter = 5000

for i in range(num_iter):
    x, y = datasets.make_moons(128, noise=.1)
    x = torch.tensor(x, dtype=torch.float32)
    y = torch.tensor(y, dtype=torch.float32).reshape(-1, 1)
    optimizer.zero_grad()
    loss = -flow.log_prob(inputs=x, context=y).mean()
    loss.backward()
    optimizer.step()
    
    if (i + 1) % 500 == 0:
        fig, ax = plt.subplots(1, 2)

        with torch.no_grad():
            zgrid0 = flow.sample(8192, context=torch.zeros(1, 1)).exp().reshape(128, 128)
            zgrid1 = flow.sample(8192, context=torch.ones(1, 1)).exp().reshape(128, 128)

        ax[0].scatter(zgrid0[:, 0], zgrid0[:, 1], c="yellow");
        ax[1].scatter(zgrid1[:, 0], zgrid1[:, 1], c="blue");

        plt.title('iteration {}'.format(i + 1))
        plt.show()

# In[ ]:

# Save the model

wrapped_model = model_from_flow(flow)
wrapped_model.eval()
with torch.no_grad():
    fake_input = torch.rand(128, 1)
    traced_model = torch.jit.trace(wrapped_model, fake_input)
    traced_model.eval()
    traced_model.save("flow_model.pt")
    torch.save(flow.state_dict(), "flow_state_dict_model.pt")
    
# Save the data from the trained model
with open('dataPyTorch_after_training_zgrid0.csv', 'w') as f:
    writer = csv.writer(f)
    writer.writerow(['x0', 'x1'])
    for i in range(len(zgrid0)):
        writer.writerow([zgrid0[i,0].cpu().numpy(), zgrid0[i,1].cpu().numpy()])
        
with open('dataPyTorch_after_training_zgrid1.csv', 'w') as f:
    writer = csv.writer(f)
    writer.writerow(['x0', 'x1'])
    for i in range(len(zgrid1)):
        writer.writerow([zgrid1[i,0].cpu().numpy(), zgrid1[i,1].cpu().numpy()])
