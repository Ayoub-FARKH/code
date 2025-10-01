// Utilitaires
const $ = (sel, ctx = document) => ctx.querySelector(sel);
const $$ = (sel, ctx = document) => Array.from(ctx.querySelectorAll(sel));

// Nav burger
(function nav() {
  const toggle = $('.nav-toggle');
  const menu = $('#nav-menu');
  if (!toggle || !menu) return;
  toggle.addEventListener('click', () => {
    const open = menu.classList.toggle('open');
    toggle.setAttribute('aria-expanded', String(open));
  });
  // close on link click (mobile)
  $('#nav-menu a').forEach(a => a.addEventListener('click', () => menu.classList.remove('open')));
  // Dropdown supprimé: plus de logique nécessaire);
    document.addEventListener('click', (e) => {
      if (!dd.contains(e.target)) {
        dd.classList.remove('open');
        ddLink.setAttribute('aria-expanded', 'false');
      }
    });
  });
    document.addEventListener('click', (e) => {
      if (!dd.contains(e.target)) {
        dd.classList.remove('open');
        ddLink.setAttribute('aria-expanded', 'false');
      }
    });
  }
})();

// Year
(function year() {
  const y = new Date().getFullYear();
  const el = $('#year');
  if (el) el.textContent = String(y);
})();

// Reveal on scroll
(function reveal() {
  const els = $$('.reveal');
  if (!('IntersectionObserver' in window) || els.length === 0) {
    els.forEach(el => el.classList.add('visible'));
    return;
  }
  const io = new IntersectionObserver((entries, obs) => {
    for (const e of entries) {
      if (e.isIntersecting) {
        e.target.classList.add('visible');
        obs.unobserve(e.target);
      }
    }
  }, { threshold: 0.12 });
  els.forEach(el => io.observe(el));
})();

// Back to top
(function backToTop() {
  const btn = $('#backToTop');
  if (!btn) return;
  window.addEventListener('scroll', () => {
    if (window.scrollY > 600) btn.classList.add('show');
    else btn.classList.remove('show');
  });
  btn.addEventListener('click', () => window.scrollTo({ top: 0, behavior: 'smooth' }));
})();

// Pré-remplir prestation depuis boutons
(function serviceButtons() {
  $('.card-cta [data-service]').forEach(btn => {
    btn.addEventListener('click', (e) => {
      const service = btn.getAttribute('data-service');
      const href = btn.getAttribute('href');
      // Si le bouton pointe vers une ancre (#contact), on préremplit; sinon on laisse la redirection normale
      if (href && href.startsWith('#')) {
        e.preventDefault();
        const s = document.querySelector('#prestation');
        if (s && service) {
          for (const opt of s.options) if (opt.textContent === service) s.value = opt.value;
        }
        document.querySelector(href)?.scrollIntoView({ behavior: 'smooth' });
      }
    });
  });
})();

// Formulaire
(function contactForm() {
  const form = $('#contact-form');
  if (!form) return;
  const fields = {
    prenom: $('#prenom'),
    nom: $('#nom'),
    email: $('#email'),
    telephone: $('#telephone'),
    prestation: $('#prestation'),
    objet: $('#objet'),
    message: $('#message'),
    consent: $('#consent'),

  };

  function setError(input, msg) {
    const small = input?.closest('.field')?.querySelector('small.error');
    if (small) small.textContent = msg || '';
  }

  function validate() {
    let ok = true;
    // Prénom / Nom
    ['prenom', 'nom'].forEach(k => {
      const v = fields[k].value.trim();
      if (v.length < 2) { setError(fields[k], 'Veuillez saisir au moins 2 caractères'); ok = false; }
      else setError(fields[k]);
    });
    // Email
    const email = fields.email.value.trim();
    const re = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    if (!re.test(email)) { setError(fields.email, 'Email invalide'); ok = false; } else setError(fields.email);
    // Téléphone
    const tel = fields.telephone.value.trim();
    const telOk = /^[0-9 +().-]{6,}$/.test(tel);
    if (!telOk) { setError(fields.telephone, 'Numéro invalide'); ok = false; } else setError(fields.telephone);
    // Prestation
    if (!fields.prestation.value) { setError(fields.prestation, 'Sélectionnez une prestation'); ok = false; } else setError(fields.prestation);
    // Objet
    if (fields.objet.value.trim().length < 3) { setError(fields.objet, 'Objet trop court'); ok = false; } else setError(fields.objet);
    // Message
    if (fields.message.value.trim().length < 10) { setError(fields.message, 'Merci de décrire votre besoin'); ok = false; } else setError(fields.message);
    // Consentement
    if (!fields.consent.checked) { fields.consent.focus(); ok = false; }

    return ok;
  }

  // Pas de mémorisation locale du brouillon (désactivé à la demande)\n  form.addEventListener('input', updateFallbacks);\n  updateFallbacks();

  // Soumission
  form.addEventListener('submit', (e) => {
    e.preventDefault();
    if (!validate()) return;

    // Par défaut: ouvre le client email avec mailto, qui est immédiat et respecte la confidentialité locale.
    updateFallbacks();
    const href = mailto.getAttribute('href');
    if (href) window.location.href = href;

    // Option future: EmailJS (désactivé). Voir doc ci-dessous.
    // emailjs.send('SERVICE_ID', 'TEMPLATE_ID', {
    //   prenom: fields.prenom.value,
    //   nom: fields.nom.value,
    //   email: fields.email.value,
    //   telephone: fields.telephone.value,
    //   prestation: fields.prestation.value,
    //   objet: fields.objet.value,
    //   message: fields.message.value,
    // }).then(() => {/* succès */}).catch(() => {/* erreur */});
  });
})();
