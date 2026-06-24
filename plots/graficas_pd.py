import matplotlib
matplotlib.use('Agg')  # Sin interfaz gráfica, guarda directo a archivo
import matplotlib.pyplot as plt
import csv
import os

# ── Leer CSVs ──────────────────────────────────────────────────────────
def leer_csv(ruta):
    with open(ruta, newline='') as f:
        reader = csv.DictReader(f)
        return list(reader)

tiempos = leer_csv('db/medicion_pd_tiempos.csv')
calidad = leer_csv('db/medicion_pd_calidad.csv')

n_vals = [int(r['n']) for r in tiempos]

# ── Gráfica 1: Tiempos de ejecución ───────────────────────────────────
t_memo     = [float(r['pd_memo'])    for r in tiempos]
t_tabu     = [float(r['pd_tabu'])    for r in tiempos]
t_puntaje  = [float(r['g_puntaje'])  for r in tiempos]
t_costo    = [float(r['g_costo'])    for r in tiempos]
t_ratio    = [float(r['g_ratio'])    for r in tiempos]
t_sinrestr = [float(r['g_sinrestr']) for r in tiempos]

fig, ax = plt.subplots(figsize=(10, 6))
ax.plot(n_vals, t_memo,     marker='o', label='PD Memoización',       color='#1A2F5A')
ax.plot(n_vals, t_tabu,     marker='s', label='PD Tabulación',        color='#2563A8')
ax.plot(n_vals, t_puntaje,  marker='^', label='Greedy Mayor Puntaje', color='#DC2626')
ax.plot(n_vals, t_costo,    marker='D', label='Greedy Menor Costo',   color='#D97706')
ax.plot(n_vals, t_ratio,    marker='x', label='Greedy Ratio',         color='#16A34A')
ax.plot(n_vals, t_sinrestr, marker='*', label='Greedy Sin Restricción',color='#9333EA')

ax.set_xlabel('Número de deportistas (n)')
ax.set_ylabel('Tiempo (segundos)')
ax.set_title('Comparación de tiempos de ejecución (W=500, K=5)')
ax.legend()
ax.grid(True, linestyle='--', alpha=0.5)
ax.set_yscale('log')
plt.tight_layout()
plt.savefig('plots/graficas/tiempos_pd_greedy.png', dpi=150)
plt.close()
print("Gráfica 1 guardada: plots/graficas/tiempos_pd_greedy.png")

# ── Gráfica 2: Calidad — puntaje obtenido ─────────────────────────────
c_memo     = [float(r['pd_memo_puntaje'])    for r in calidad]
c_tabu     = [float(r['pd_tabu_puntaje'])    for r in calidad]
c_puntaje  = [float(r['g_puntaje_puntaje'])  for r in calidad]
c_costo    = [float(r['g_costo_puntaje'])    for r in calidad]
c_ratio    = [float(r['g_ratio_puntaje'])    for r in calidad]
c_sinrestr = [float(r['g_sinrestr_puntaje']) for r in calidad]

fig, ax = plt.subplots(figsize=(10, 6))
ax.plot(n_vals, c_memo,     marker='o', label='PD Memoización (K=5)',  color='#1A2F5A')
ax.plot(n_vals, c_tabu,     marker='s', label='PD Tabulación (K=5)',   color='#2563A8')
ax.plot(n_vals, c_puntaje,  marker='^', label='Greedy Mayor Puntaje',  color='#DC2626')
ax.plot(n_vals, c_costo,    marker='D', label='Greedy Menor Costo',    color='#D97706')
ax.plot(n_vals, c_ratio,    marker='x', label='Greedy Ratio',          color='#16A34A')
ax.plot(n_vals, c_sinrestr, marker='*', label='Greedy Sin Restricción',color='#9333EA')

ax.set_xlabel('Número de deportistas (n)')
ax.set_ylabel('Puntaje total obtenido')
ax.set_title('Comparación de calidad de solución (W=500, K=5)')
ax.legend()
ax.grid(True, linestyle='--', alpha=0.5)
plt.tight_layout()
plt.savefig('plots/graficas/calidad_pd_greedy.png', dpi=150)
plt.close()
print("Gráfica 2 guardada: plots/graficas/calidad_pd_greedy.png")

# ── Gráfica 3: Cantidad de deportistas seleccionados ──────────────────
k_memo     = [int(r['pd_memo_k'])    for r in calidad]
k_puntaje  = [int(r['g_puntaje_k'])  for r in calidad]
k_costo    = [int(r['g_costo_k'])    for r in calidad]
k_ratio    = [int(r['g_ratio_k'])    for r in calidad]
k_sinrestr = [int(r['g_sinrestr_k']) for r in calidad]

fig, ax = plt.subplots(figsize=(10, 6))
ax.plot(n_vals, k_memo,     marker='o', label='PD (K fijo=5)',         color='#1A2F5A')
ax.plot(n_vals, k_puntaje,  marker='^', label='Greedy Mayor Puntaje',  color='#DC2626')
ax.plot(n_vals, k_costo,    marker='D', label='Greedy Menor Costo',    color='#D97706')
ax.plot(n_vals, k_ratio,    marker='x', label='Greedy Ratio',          color='#16A34A')
ax.plot(n_vals, k_sinrestr, marker='*', label='Greedy Sin Restricción',color='#9333EA')

ax.set_xlabel('Número de deportistas (n)')
ax.set_ylabel('Deportistas seleccionados')
ax.set_title('Cantidad de deportistas seleccionados por algoritmo (W=500, K=5)')
ax.legend()
ax.grid(True, linestyle='--', alpha=0.5)
plt.tight_layout()
plt.savefig('plots/graficas/cantidad_pd_greedy.png', dpi=150)
plt.close()
print("Gráfica 3 guardada: plots/graficas/cantidad_pd_greedy.png")

print("\n[OK] Todas las graficas generadas.")