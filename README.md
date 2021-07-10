# oxycleaned 
A CLI program for simulating OSRS duels and reporting staking win-loss odds.

--- Notes About the Program ---  

The calculator assumes the following:
* You will not gain +1 damage on your max hit if you use controlled.  
* You always swap between Accurate and Defensive accordingly.
* Your opponent will always swap attacks and defenses according to their breakpoint or not as well.
* You and your opponent are both using Abyssal Tentacle.  
* Standard duel ruleset applies (No armor, No food, No special attacks, No weapon switch, etc.)

The calculator does have support for Abyssal Whip, but you must change the source code. I left it out of the runtime because nobody really does regular whip duels anymore. The calculator runs 1,000,000 simulations per scenario, which may take a few seconds to calculate. The more simulations the better, you can change this setting in the source code as well. I reccomend no lower than one million simulations.

This code may be have bugs or other unknown problems. Use it at your own risk.  

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
