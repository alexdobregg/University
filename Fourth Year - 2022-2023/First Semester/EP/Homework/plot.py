import matplotlib.pyplot as plt

data = {'h1(10.10.101.2)':690,
        'h2(10.10.101.3)':691,
        'h3(10.10.102.2)':243,
        'h4(10.10.102.3)':713,
        'h5(10.10.103.2)':337,
        'h6(10.10.103.3)':175}

machines = list(data.keys())
time = list(data.values())
  
fig = plt.figure(figsize = (10, 6))
 
# creating the bar plot
# plt.bar(courses, values, color ='maroon',
#         width = 0.3)
plt.bar(machines[0], time[0], color = 'green', width = 0.3)
plt.bar(machines[1], time[1], color = 'green', width = 0.3)
plt.bar(machines[2], time[2], color = 'green', width = 0.3)
plt.bar(machines[3], time[3], color = 'green', width = 0.3)
plt.bar(machines[4], time[4], color = 'green', width = 0.3)
plt.bar(machines[5], time[5], color = 'green', width = 0.3)
 
plt.xlabel("Machines and their coresponding IP", fontweight ='bold', fontsize = 15)
plt.ylabel("Time(s)", fontweight ='bold', fontsize = 15)
plt.title("Time elapsed handling 308 requests", fontweight ='bold', fontsize = 15)
plt.show()