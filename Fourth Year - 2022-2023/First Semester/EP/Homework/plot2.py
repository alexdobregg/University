import matplotlib.pyplot as plt

import matplotlib.pyplot as plt

import numpy as np


# req = {'h1':150,
#         'h2':150,
#         'h3':150,
#         'h4':150,
#         'h5':150,
#         'h6':150}

# minT = {'h1':70,
#         'h2':5,
#         'h3':107,
#         'h4':211,
#         'h5':80,
#         'h6':94}

# maxT = {'h1':999,
#         'h2':997,
#         'h3':773,
#         'h4':806,
#         'h5':932,
#         'h6':684}

averageT1 = [429,281,201,245,202,292]

# req = {'h1':82,
#         'h2':82,
#         'h3':246,
#         'h4':82,
#         'h5':164,
#         'h6':244}

# minT = {'h1':111,
#         'h2':99,
#         'h3':95,
#         'h4':188,
#         'h5':70,
#         'h6':13}

# maxT = {'h1':348,
#         'h2':641,
#         'h3':879,
#         'h4':847,
#         'h5':641,
#         'h6':886}

averageT2 = [192,315,342,411,242,311]

# req = {'h1':157,
#         'h2':143,
#         'h3':154,
#         'h4':146,
#         'h5':149,
#         'h6':151}

# minT = {'h1':6,
#         'h2':18,
#         'h3':124,
#         'h4':211,
#         'h5':134,
#         'h6':151}

# maxT = {'h1':993,
#         'h2':935,
#         'h3':732,
#         'h4':953,
#         'h5':800,
#         'h6':540}

averageT3 = [453, 455, 210, 260, 182, 177]



print(np.mean(averageT1))
print(np.mean(averageT2))
print(np.mean(averageT3))



# machines = list(req.keys())
# nrReq = list(req.values())
# mint = list(minT.values())
# maxt = list(maxT.values())
# averaget = list(averageT.values())
  
# fig, ax = plt.subplots(1, 3)
# ax[0].set(title='Number of requests for each machine', ylabel='Requests', xlabel = 'Machine')
# ax[1].set(title='Min/Max time for each machine', ylabel='Time(ms)', xlabel = 'Machine')
# ax[2].set(title='Average time for each machine', ylabel='Time(ms)', xlabel = 'Machine')

# x_axis = range(1, 7)
# x_axis_1 = [x - 0.20 for x in x_axis]
# x_axis_2 = [x + 0.20 for x in x_axis]

# ax[0].bar(machines, nrReq, color = 'blue', width = 0.4)
# ax[1].bar(x_axis_1, mint, color = 'blue', width=0.4)
# ax[1].bar(x_axis_2, maxt, color = 'red', width=0.4)
# ax[2].bar(machines, averaget, color = "blue", width=0.4)


# plt.show()
