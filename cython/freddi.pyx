# distutils: language = c++

from cython.operator cimport dereference

import numpy as np
cimport numpy as cnp

from freddi cimport *


cdef class Arguments:
    cdef FreddiArguments* cpp_args

    def __cinit__(
        self, *, bint cgs=True,
        double alpha=default_alpha, double Mx=default_Mx, double kerr=default_kerr, double Mopt=default_Mopt, double period=default_period, rin=None, rout=None,
        string opacity=default_opacity, string boundcond=default_boundcond, double Thot=default_Thot, string initialcond=default_initialcond, double F0=default_F0, double powerorder=default_powerorder, double gaussmu=default_gaussmu, double gausssigma=default_gausssigma, Mdisk0=None, Mdot0=None,
        double Cirr=default_Cirr, string irrfactortype=default_irrfactortype,
        double colourfactor=default_colourfactor, double emin=default_emin, double emax=default_emax, double inclination=default_inclination, double distance=default_distance, vector[double] lambdas=[],
        double time=default_time, double tau=default_tau, unsigned int Nx=default_Nx, string gridscale=default_gridscale, eps=None
    ):
        if not cgs:
            Mx = sunToGram(Mx)
            Mopt = sunToGram(Mopt)
            period = dayToS(period)
            if rin is not None:
                rin = rgToCm(rin, Mx)
            if rout is not None:
                rout = sunToCm(rout)
            emin = kevToHertz(emin)
            emax = kevToHertz(emax)
            distance = kpcToCm(distance)
            for i in range(lambdas.size()):
                lambdas[i] = angstromToCm(lambdas[i])
            time = dayToS(time)
            tau = dayToS(tau)
        cdef GeneralArguments* general = new GeneralArguments(b'', b'', False)
        cdef BasicDiskBinaryArguments* basic
        if rin is None and rout is None:
            basic = new BasicDiskBinaryArguments(constructWithoutRinRout(alpha, Mx, kerr, Mopt, period))
        elif rin is None:
            basic = new BasicDiskBinaryArguments(constructWithoutRin(alpha, Mx, kerr, Mopt, period, rout))
        elif rout is None:
            basic = new BasicDiskBinaryArguments(constructWithoutRout(alpha, Mx, kerr, Mopt, period, rin))
        else:
            basic = new BasicDiskBinaryArguments(alpha, Mx, kerr, Mopt, period, rin, rout)
        cdef bint is_Mdisk0_specified = Mdisk0 is not None
        if Mdisk0 is None:
            Mdisk0 = -1.0
        cdef bint is_Mdot0_specified = Mdot0 is not None
        if Mdot0 is None:
            Mdot0 = -1.0
        cdef DiskStructureArguments* disk = new DiskStructureArguments(dereference(basic), opacity, boundcond, Thot, initialcond, F0, powerorder, gaussmu, gausssigma, is_Mdisk0_specified, is_Mdot0_specified, Mdisk0, Mdot0)
        cdef SelfIrradiationArguments* irr = new SelfIrradiationArguments(Cirr, irrfactortype)
        cdef FluxArguments* flux = new FluxArguments(colourfactor, emin, emax, inclination, distance, lambdas)
        cdef CalculationArguments* calc
        if eps is None:
            calc = new CalculationArguments(time, tau, Nx, gridscale)
        else:
            calc = new CalculationArguments(time, tau, Nx, gridscale, eps)
        self.cpp_args = new FreddiArguments(general, basic, disk, irr, flux, calc)

    def __dealloc__(self):
        del self.cpp_args

    @property
    def time(self):
        return self.cpp_args.calc.get().time

    @property
    def lambdas(self):
        cdef const double* data = self.cpp_args.flux.get().lambdas.data()
        cdef size_t size = self.cpp_args.flux.get().lambdas.size()
        if size == (<size_t> 0):
            return np.array([], dtype=np.float)
        arr = np.asarray(<const double[:size]> data)
        arr.flags.writeable = False
        return arr


cdef class State:
    cdef FreddiState* cpp_state

    def __dealloc__(self):
        if self.cpp_state:
            del self.cpp_state

    @property
    def Mdot_in(self) -> double:
       return self.cpp_state.get_Mdot_in()

    @property
    def Mdot(self) -> double:
        return self.Mdot_in

    @property
    def Mdot_out(self) -> double:
        return self.cpp_state.get_Mdot_out()

    @property
    def Lx(self) -> double:
        return self.cpp_state.get_Lx()
        
    @property
    def t(self) -> double:
        return self.cpp_state.get_t()

    @property
    def i_t(self) -> int:
        return self.cpp_state.get_i_t()

    @property
    def Nx(self) -> int:
        return self.cpp_state.get_Nx()

    @property
    def h(self) -> np.ndarray[np.float]:
        cdef const double* data = self.cpp_state.get_h().data()
        cdef size_t size = self.cpp_state.get_h().size()
        arr = np.asarray(<const double[:size]> data)
        arr.flags.writeable = False
        return arr

    @property
    def R(self) -> np.ndarray[np.float]:
        cdef const double* data = self.cpp_state.get_R().data()
        cdef size_t size = self.cpp_state.get_R().size()
        arr = np.asarray(<const double[:size]> data)
        arr.flags.writeable = False
        return arr

    @property
    def F(self) -> np.ndarray[np.float]:
        cdef const double* data = self.cpp_state.get_F().data()
        cdef size_t size = self.cpp_state.get_F().size()
        arr = np.asarray(<const double[:size]> data)
        arr.flags.writeable = False
        return arr

    @property
    def W(self) -> np.ndarray[np.float]:
        cdef const double* data = self.cpp_state.get_W().data()
        cdef size_t size = self.cpp_state.get_W().size()
        arr = np.asarray(<const double[:size]> data)
        arr.flags.writeable = False
        return arr

    @property
    def Tph(self) -> np.ndarray[np.float]:
        cdef const double* data = self.cpp_state.get_Tph().data()
        cdef size_t size = self.cpp_state.get_Tph().size()
        arr = np.asarray(<const double[:size]> data)
        arr.flags.writeable = False
        return arr

    @property
    def Tph_vis(self) -> np.ndarray[np.float]:
        cdef const double* data = self.cpp_state.get_Tph_vis().data()
        cdef size_t size = self.cpp_state.get_Tph_vis().size()
        arr = np.asarray(<const double[:size]> data)
        arr.flags.writeable = False
        return arr

    @property
    def Tirr(self) -> np.ndarray[np.float]:
        cdef const double* data = self.cpp_state.get_Tirr().data()
        cdef size_t size = self.cpp_state.get_Tirr().size()
        arr = np.asarray(<const double[:size]> data)
        arr.flags.writeable = False
        return arr

    @property
    def Cirr(self) -> np.ndarray[np.float]:
        cdef const double* data = self.cpp_state.get_Cirr().data()
        cdef size_t size = self.cpp_state.get_Cirr().size()
        arr = np.asarray(<const double[:size]> data)
        arr.flags.writeable = False
        return arr

    @property
    def Sigma(self) -> np.ndarray[np.float]:
        cdef const double* data = self.cpp_state.get_Sigma().data()
        cdef size_t size = self.cpp_state.get_Sigma().size()
        arr = np.asarray(<const double[:size]> data)
        arr.flags.writeable = False
        return arr

    @property
    def Height(self) -> np.ndarray[np.float]:
        cdef const double* data = self.cpp_state.get_Height().data()
        cdef size_t size = self.cpp_state.get_Height().size()
        arr = np.asarray(<const double[:size]> data)
        arr.flags.writeable = False
        return arr

    @property
    def last_h(self) -> double:
        return self.cpp_state.get_h().back()

    @property
    def last_R(self) -> double:
        return self.cpp_state.get_R().back()

    @property
    def last_F(self) -> double:
        return self.cpp_state.get_F().back()

    @property
    def last_W(self) -> double:
        return self.cpp_state.get_W().back()

    @property
    def last_Tph(self) -> double:
        return self.cpp_state.get_Tph().back()

    @property
    def last_Tph_vis(self) -> double:
        return self.cpp_state.get_Tph_vis().back()

    @property
    def last_Tirr(self) -> double:
        return self.cpp_state.get_Tirr().back()

    @property
    def last_Cirr(self) -> double:
        return self.cpp_state.get_Cirr().back()

    @property
    def last_Sigma(self) -> double:
        return self.cpp_state.get_Sigma().back()

    @property
    def last_Height(self) -> double:
        return self.cpp_state.get_Height().back()

    @property
    def mU(self) -> double:
        return self.cpp_state.mU()

    @property
    def mB(self) -> double:
        return self.cpp_state.mB()

    @property
    def mV(self) -> double:
        return self.cpp_state.mV()

    @property
    def mR(self) -> double:
        return self.cpp_state.mR()

    @property
    def mI(self) -> double:
        return self.cpp_state.mI()

    @property
    def mJ(self) -> double:
        return self.cpp_state.mJ()

    @property
    def Mdisk(self) -> double:
        return self.cpp_state.Mdisk()

    def flux(self, double lmbd) -> double:
        return self.cpp_state.flux(lmbd)
   

cdef State state_from_cpp(const FreddiState& cpp_state):
    cdef State state = State()
    state.cpp_state = new FreddiState(cpp_state)
    return state


cdef class EvolutionResults:
    cdef vector[FreddiState] cpp_states
    cdef object[:] states

    def __cinit__(self, Freddi freddi):
        self.cpp_states = freddi.evolution.evolve()
        self.states = np.empty(<Py_ssize_t> self.cpp_states.size(), dtype=object)
        cdef Py_ssize_t i
        for i in range(self.states.size):
            self.states[i] = state_from_cpp(self.cpp_states[i])

    def flux(self, double lmbd) -> double:
        arr = np.zeros((self.states.size, 1), dtype=np.float)
        cdef size_t i
        for i in range(self.states.size):
            arr[i] = self.states[i].flux(lmbd)
        return arr

    def __getattr__(self, attr) -> np.ndarray:
        value = getattr(self.states[0], attr)
        cdef Py_ssize_t size = 1
        dtype = type(value)
        if isinstance(value, np.ndarray):
            size = value.size
            dtype = value.dtype
        arr = np.zeros((self.states.size, size), dtype=dtype)
        arr[0, :] = value
        cdef size_t i
        for i in range(1, self.states.size):
            value = getattr(self.states[i], attr)
            if size == 1:
                arr[i, 0] = value
            else:
                arr[i, :value.size] = value
        return arr


cdef class Freddi:
    cdef FreddiEvolution* evolution
    cdef Arguments args
    cdef double time

    def __cinit__(self, Arguments args):
        self.args = args
        self.evolution = new FreddiEvolution(dereference(self.args.cpp_args))
        self.time = self.args.cpp_args.calc.get().time

    def __dealloc__(self):
        del self.evolution

    @property
    def Nt(self) -> int:
        return self.evolution.Nt

    cdef State get_state(self):
        return state_from_cpp(self.evolution.get_state())

    def __iter__(self):
        state = self.get_state()
        yield state
        while state.t <= self.time:
            self.evolution.step()
            state = self.get_state()
            yield state

    def evolve(self):
        return EvolutionResults(self)

