# Script for finding all possible character ships (including polycules).

cast = "Fang", "Trish", "Reed", "Naser", "Naomi", "Rosa", "Sage", "Stella", "Swamp1", "Swamp2", "Swamp3", "Alvin", "Leo"
n = len(cast)

# Nuh uh
def forbidden (ship):
  return "Fang" in ship and "Naser" in ship

# Iterate over all possible polycules.
# Inputs:
#     - The cast members available for the polycules
#     - Number of parallel polycules to have (exact number).
#       If unspecified, then check all numbers of parallel polycules.
# NOTE: Does not remove duplicate entries, these need to be filtered out
#       afterwards.
# Returns iterations of (combo, leftover members).
def iter_polycules (group,num_parallel=None):
  from itertools import combinations
  # If specific number of parallel ships is requested
  if num_parallel is not None:
    # Base case (single polycule)
    if num_parallel == 1:
      for size in range(2,len(group)+1):
        for ship in combinations(sorted(group),size):
          if not forbidden(ship): yield (ship,), set(group)-set(ship)
    # Multiple polycules
    else:
      for some_ships, remaining in iter_polycules(group,num_parallel-1):
        for last_ship, leftover in iter_polycules(remaining,1):
          yield some_ships + last_ship, leftover
  # If no specific number requested, then look at all possible number of
  # parallel ships.
  # Need at least 2 members in each grouping, so up to N/2 total number
  # of parallel groupings.
  else:
    for num_parallel in range(1,len(group)//2+1):
      for ships, leftover in iter_polycules(group,num_parallel):
        yield ships, leftover

# Get all *unique* combinations of polycules.
def polycules (group):
  combos = set()
  for ships, leftover in iter_polycules(group):
    ships = tuple(sorted(ships))
    combos.add(ships)
  for ships in sorted(combos):
    yield ships

count = 0
for ships in polycules(cast):
  count += 1
  #print (*ships)

print ()
print ("Total number:", count)

