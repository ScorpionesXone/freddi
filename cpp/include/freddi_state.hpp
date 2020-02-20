#ifndef FREDDI_FREDDI_STATE_HPP
#define FREDDI_FREDDI_STATE_HPP

#include <functional>  // bind, function
#include <vector>

#include <boost/optional.hpp>

#include <arguments.hpp>
#include <passband.hpp>
#include <spectrum.hpp>
#include <star.hpp>
#include <util.hpp>


class FreddiState {
protected:
	typedef std::function<vecd (const vecd&, const vecd&, size_t, size_t)> wunc_t;
public:
	enum DiskIntegrationRegion {
		HotRegion,
		ColdRegion,
	};
private:
	class BasicWind {
	protected:
		vecd A_, B_, C_;
	public:
		explicit BasicWind(const FreddiState&);
		virtual ~BasicWind() = 0;
		virtual BasicWind* clone() const = 0;
		virtual void update(const FreddiState&) {}
		inline const vecd& A() const { return A_; }
		inline const vecd& B() const { return B_; }
		inline const vecd& C() const { return C_; }
	};

	class NoWind: public BasicWind {
	public:
		explicit NoWind(const FreddiState& state): BasicWind(state) {}
		~NoWind() override = default;
		NoWind(const NoWind&) = default;
		virtual NoWind* clone() const override { return new NoWind(*this); }
	};

	class SS73CWind: public BasicWind {
	public:
		explicit SS73CWind(const FreddiState& state);
		~SS73CWind() override = default;
		SS73CWind(const SS73CWind&) = default;
		virtual SS73CWind* clone() const override { return new SS73CWind(*this); }
	};

	class Cambier2013Wind: public BasicWind {
	private:
		// windparams
		const double kC;
		const double R_IC2out;
	public:
		explicit Cambier2013Wind(const FreddiState& state);
		~Cambier2013Wind() override = default;
		Cambier2013Wind(const Cambier2013Wind&) = default;
		virtual Cambier2013Wind* clone() const override { return new Cambier2013Wind(*this); }
	};

	class __testA__Wind: public BasicWind {
	private:
		// windparams
		const double kA;
	public:
		explicit __testA__Wind(const FreddiState& state);
		~__testA__Wind() override = default;
		__testA__Wind(const __testA__Wind&) = default;
		virtual __testA__Wind* clone() const override { return new __testA__Wind(*this); }
	};

	class __testB__Wind: public BasicWind {
	private:
		// windparams
		const double kB;
	public:
		explicit __testB__Wind(const FreddiState& state);
		~__testB__Wind() override = default;
		__testB__Wind(const __testB__Wind&) = default;
		virtual __testB__Wind* clone() const override { return new __testB__Wind(*this); }
	};

	class __testC__Wind: public BasicWind {
	private:
		// windparams
		const double kC;
	public:
		explicit __testC__Wind(const FreddiState& state);
		~__testC__Wind() override = default;
		__testC__Wind(const __testC__Wind&) = default;
		virtual __testC__Wind* clone() const override { return new __testC__Wind(*this); }
	};

	class __testC_q0_Shields1986__: public BasicWind {
	private:
		// windparams
		const double kC;
		const double R_windmin2out;
	public:
		explicit __testC_q0_Shields1986__(const FreddiState& state);
		~__testC_q0_Shields1986__() override = default;
		__testC_q0_Shields1986__(const __testC_q0_Shields1986__&) = default;
		virtual __testC_q0_Shields1986__* clone() const override { return new __testC_q0_Shields1986__(*this); }
		virtual void update(const FreddiState&) override;
	};

	class __Unstedy_Test_Hunter__: public BasicWind {
	private:
		// windparams
		const double f_X;
		const double X_f;
		const double T_iC;
	public:
		explicit __Unstedy_Test_Hunter__(const FreddiState& state);
		~__Unstedy_Test_Hunter__() override = default;
		__Unstedy_Test_Hunter__(const __Unstedy_Test_Hunter__&) = default;
		virtual __Unstedy_Test_Hunter__* clone() const override { return new __Unstedy_Test_Hunter__(*this); }
		virtual void update(const FreddiState&) override;
	};

	class  __Agnieszka_Wind_Paper__: public BasicWind {
	private:
		// windparams
		const double A_0;
		const double B_1;
	public:
		explicit  __Agnieszka_Wind_Paper__(const FreddiState& state);
		~__Agnieszka_Wind_Paper__() override = default;
		__Agnieszka_Wind_Paper__(const  __Agnieszka_Wind_Paper__&) = default;
		virtual  __Agnieszka_Wind_Paper__* clone() const override { return new  __Agnieszka_Wind_Paper__(*this); }
		virtual void update(const FreddiState&) override;
	};

	class __Woods_McKee_Klein__: public BasicWind {
	private:
		// windparams
		const double C_0;
		const double T_iC;
	public:
		explicit __Woods_McKee_Klein__(const FreddiState& state);
		~__Woods_McKee_Klein__() override = default;
		__Woods_McKee_Klein__(const __Woods_McKee_Klein__&) = default;
		virtual __Woods_McKee_Klein__* clone() const override { return new __Woods_McKee_Klein__(*this); }
		virtual void update(const FreddiState&) override;
	};

	class __Woods_Which_Shields__ : public BasicWind {
	private:
		// windparams
		const double Xi_max;
		const double T_iC;
	public:
		explicit __Woods_Which_Shields__(const FreddiState& state);
		~__Woods_Which_Shields__() override = default;
		__Woods_Which_Shields__(const __Woods_Which_Shields__&) = default;
		virtual __Woods_Which_Shields__* clone() const override { return new __Woods_Which_Shields__(*this); }
		virtual void update(const FreddiState&) override;
	};

protected:
	class BasicRadiationAngularDistribution {
	public:
		virtual ~BasicRadiationAngularDistribution() = 0;
		virtual double operator()(double mu) const = 0; // mu = cos(angle between ray and normal)
	};

	class IsotropicRadiationAngularDistribution: public BasicRadiationAngularDistribution {
	public:
		~IsotropicRadiationAngularDistribution() override = default;
		double operator()(double mu) const override;
	};

	class PlaneRadiationAngularDistribution: public BasicRadiationAngularDistribution {
	public:
		~PlaneRadiationAngularDistribution() override = default;
		double operator()(double mu) const override;
	};

private:
	class DiskStructure {
	public:
		FreddiArguments args;
		size_t Nt;
		size_t Nx;
		double GM;
		double R_g;
		double eta;
		double semiaxis;
		double inclination;
		double cosi;
		double distance;
		double cosiOverD2;
		OpacityRelated oprel;
		vecd h;
		vecd R;
		wunc_t wunc;
		DiskStructure(const FreddiArguments& args, const wunc_t& wunc);
	private:
		static vecd initialize_h(const FreddiArguments& args, size_t Nx);
		static vecd initialize_R(const vecd& h, double GM);
	};

	class CurrentState {
	public:
		double F_in = 0;
		double Mdot_out;
		double Mdot_in_prev = -INFINITY;
		double t = 0;
		size_t i_t = 0;
		size_t first = 0;
		size_t last;
		vecd F;
		explicit CurrentState(const DiskStructure& str);
		CurrentState(const CurrentState&) = default;
		CurrentState& operator=(const CurrentState&) = default;
	private:
		static vecd initializeF(const DiskStructure& str);
	};

	struct DiskOptionalStructure {
		boost::optional<double> Mdisk;
		boost::optional<double> Lx;
		boost::optional<double> Mdot_wind;
		boost::optional<double> mU, mB, mV, mR, mI, mJ;
		boost::optional<vecd> W, Tph, Qx, Tph_vis, Tph_X, Tirr, Kirr, Sigma, Height;
	};

protected:
	std::shared_ptr<const DiskStructure> str_;
	CurrentState current_;
	DiskOptionalStructure opt_str_;
	std::unique_ptr<BasicWind> wind_;
	std::shared_ptr<BasicRadiationAngularDistribution> angular_dist_disk_;
	IrradiatedStar star_;
public:
	FreddiState(const FreddiArguments& args, const wunc_t& wunc);
	explicit FreddiState(const FreddiState&);
	FreddiState(FreddiState&&) = delete;
	FreddiState& operator=(const FreddiState&) = delete;
	FreddiState& operator=(FreddiState&&) = delete;
	virtual void step(double tau);
private:
	void initializeWind();
// str_
public:
	inline size_t Nt() const { return str_->Nt; }
	inline size_t Nx() const { return str_->Nx; }
	inline double GM() const { return str_->GM; }
	inline double R_g() const { return str_->R_g; }
	inline double eta() const { return str_->eta; }
	inline double semiaxis() const { return str_->semiaxis; }
	inline double inclination() const { return str_->inclination; }
	inline double cosi() const { return str_->cosi; }
	inline double distance() const { return str_->distance; }
	inline double cosiOverD2() const { return str_->cosiOverD2; }
	inline const OpacityRelated& oprel() const { return str_->oprel; }
	inline const wunc_t& wunc() const { return str_->wunc; }
	inline const FreddiArguments& args() const { return str_->args; }
	inline const vecd& h() const { return str_->h; }
	inline const vecd& R() const { return str_->R; }
	inline const vecd& lambdas() const { return str_->args.flux->lambdas; }
	inline Star& star() { return star_; }
	void replaceArgs(const FreddiArguments& args);  // Danger!
// current_
public:
	inline double Mdot_out() const { return current_.Mdot_out; }
	inline double F_in() const { return current_.F_in; }
	inline const vecd& F() const { return current_.F; }
	inline double t() const { return current_.t; }
	inline size_t i_t() const { return current_.i_t; };
	inline size_t first() const { return current_.first; }
	inline size_t last() const { return current_.last; }
	inline double Mdot_in_prev() const { return current_.Mdot_in_prev; }
protected:
	inline void set_Mdot_in_prev(double Mdot_in) { current_.Mdot_in_prev = Mdot_in; }
	inline void set_Mdot_in_prev() { set_Mdot_in_prev(Mdot_in()); }
	virtual IrradiatedStar::sources_t star_irr_sources();
public:
	inline double omega_R(double r) const { return std::sqrt(GM() / (r*r*r)); }
	inline double omega_i(size_t i) const { return omega_R(R()[i]); }
	virtual double Mdot_in() const;
	virtual double Lbol_disk() const;
// wind_
public:
	virtual vecd windA() const { return wind_->A(); }
	virtual vecd windB() const { return wind_->B(); }
	virtual vecd windC() const { return wind_->C(); }
// angular_dist_disk_
protected:
	static std::shared_ptr<BasicRadiationAngularDistribution> initializeAngularDist(const std::string& angular_dist_type);
public:
	inline double angular_dist_disk(const double mu) const { return (*angular_dist_disk_)(mu); }
// opt_str_
protected:
	virtual void invalidate_optional_structure();

	template <DiskIntegrationRegion Region> size_t region_first() const {
		if constexpr(Region == HotRegion) {
			return first();
		} else if constexpr(Region == ColdRegion) {
			return last() + 1;
		} else {
			static_assert("Wrong Region template argument");
		}
	}
	template <DiskIntegrationRegion Region> size_t region_last() const {
		if constexpr(Region == HotRegion) {
			return last();
		} else if constexpr(Region == ColdRegion) {
			return Nx() - 1;
		} else {
			static_assert("Wrong Region template argument");
		}
	}
	template <DiskIntegrationRegion Region> double integrate(const vecd& values) const {
		return disk_radial_trapz(R(), values, region_first<Region>(), region_last<Region>());
	}
	template <DiskIntegrationRegion Region> double integrate(const std::function<double (size_t)>& func) const {
		return disk_radial_trapz(R(), func, region_first<Region>(), region_last<Region>());
	}
	template <DiskIntegrationRegion Region> double integrate(const vecd& x, const std::function<double (size_t)>& func) const {
		return trapz(x, func, region_first<Region>(), region_last<Region>());
	}
	template <DiskIntegrationRegion Region> double lazy_integrate(boost::optional<double>& x, const vecd& values) {
		if (!x) {
			x = integrate<Region>(values);
		}
		return *x;
	}
	template<DiskIntegrationRegion Region> double lazy_integrate(boost::optional<double> &opt, const vecd& x, const std::function<double (size_t)>& values) {
		if (!opt) {
			opt = integrate<Region>(x, values);
		}
		return *opt;
	}
	template <DiskIntegrationRegion Region> double I_lambda(double lambda) {
		const vecd* T;
		if constexpr(Region == HotRegion) {
			T = &Tph();
		} else if constexpr(Region == ColdRegion) {
			T = &Tirr();
		} else {
			static_assert("Wrong Region template argument");
		}
		return integrate<Region>([T, lambda](const size_t i) -> double { return Spectrum::Planck_lambda((*T)[i], lambda); });
	}
	double lazy_magnitude(boost::optional<double>& m, double lambda, double F0);
	virtual const vecd& Qx();
public:
	double Lx();
	const vecd& W();
	const vecd& Sigma();
	const vecd& Tph();
	const vecd& Tph_vis();
	const vecd& Tph_X();
	const vecd& Tirr();
	const vecd& Kirr();
	const vecd& Height();
	double Luminosity(const vecd& T, double nu1, double nu2) const;
	inline double magnitude(const double lambda, const double F0) {
		return -2.5 * std::log10(I_lambda<HotRegion>(lambda) * cosiOverD2() / F0);
	}
	template <DiskIntegrationRegion Region> double flux_region(double lambda) {
		return I_lambda<Region>(lambda) * m::pow<2>(lambda) / GSL_CONST_CGSM_SPEED_OF_LIGHT * cosiOverD2();
	}
	template <DiskIntegrationRegion Region> double flux_region(const Passband& passband) {
		const double intens = trapz(
				passband.lambdas,
				[this, &passband](const size_t i) -> double {
					return I_lambda<Region>(passband.lambdas[i]) * passband.transmissions[i];
				},
				0,
				passband.data.size() - 1);
		return intens * cosiOverD2() / passband.t_dnu;
	}
	inline double flux(const double lambda) { return flux_region<HotRegion>(lambda); }
	inline double flux(const Passband& passband) { return flux_region<HotRegion>(passband); }
	double flux_star(double lambda, double phase);
	double flux_star(const Passband& passband, double phase);
	inline double mU() { return lazy_magnitude(opt_str_.mU, lambdaU, irr0U); }
	inline double mB() { return lazy_magnitude(opt_str_.mB, lambdaB, irr0B); }
	inline double mV() { return lazy_magnitude(opt_str_.mV, lambdaV, irr0V); }
	inline double mR() { return lazy_magnitude(opt_str_.mR, lambdaR, irr0R); }
	inline double mI() { return lazy_magnitude(opt_str_.mI, lambdaI, irr0I); }
	inline double mJ() { return lazy_magnitude(opt_str_.mJ, lambdaJ, irr0J); }
	inline double Mdisk() { return lazy_integrate<HotRegion>(opt_str_.Mdisk, Sigma()); }
	double Mdot_wind();
};

#endif //FREDDI_FREDDI_STATE_HPP
