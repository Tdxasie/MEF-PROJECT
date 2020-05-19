import numpy as np
import matplotlib.pyplot as plt


def get_data(filename):
    data = np.loadtxt("RESULTS/" + filename)
    return data[:,0], data[:,1]

h = [2**i for i in range(1, 7)]

# ==============================================
# Exemple 1 domaine 1
# ==============================================


# ========= maille triangle ============

plt.figure(1)

errQuad, errMax = get_data("results_ex1_dom1_t")
plt.plot(h, errQuad, label='Erreur Quadratique relative')
plt.plot(h, errMax, label='Erreur Maximum relative')
plt.xlabel('h')
plt.ylabel('Erreurs')
plt.title("Erreurs pour l'exemple 1 domaine 1, maille triangle")
plt.legend()

# ========= maille quadrangle ============

plt.figure(2)

errQuad, errMax = get_data("results_ex1_dom1_q")
plt.plot(h, errQuad, label='Erreur Quadratique relative')
plt.plot(h, errMax, label='Erreur Maximum relative')
plt.xlabel('h')
plt.ylabel('Erreurs')
plt.title("Erreurs pour l'exemple 1 domaine 1, maille quadrangle")
plt.legend()


# ==============================================
# Exemple 2 domaine 1
# ==============================================


# ========= maille triangle ============

plt.figure(3)

errQuad, errMax = get_data("results_ex2_dom1_t")
plt.plot(h, errQuad, label='Erreur Quadratique relative')
plt.plot(h, errMax, label='Erreur Maximum relative')
plt.xlabel('h')
plt.ylabel('Erreurs')
plt.title("Erreurs pour l'exemple 2 domaine 1, maille triangle")
plt.legend()

# ========= maille quadrangle ============

plt.figure(4)

errQuad, errMax = get_data("results_ex2_dom1_q")
plt.plot(h, errQuad, label='Erreur Quadratique relative')
plt.plot(h, errMax, label='Erreur Maximum relative')
plt.xlabel('h')
plt.ylabel('Erreurs')
plt.title("Erreurs pour l'exemple 2 domaine 1, maille quadrangle")
plt.legend()


plt.show()